/**
 * A65
 * Copyright (C) 2018 David Jolly
 *
 * A65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../inc/a65_tree.h"
#include "../inc/a65_utility.h"

a65_tree::a65_tree(
	__in_opt int type
	) :
		m_node(A65_UUID_INVALID),
		m_node_root(A65_UUID_INVALID),
		m_type(type)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_TREE_STRING(type));
	A65_DEBUG_EXIT();
}

a65_tree::a65_tree(
	__in const a65_tree &other
	) :
		a65_id(other),
		m_node(other.m_node),
		m_node_map(other.m_node_map),
		m_node_root(other.m_node_root),
		m_type(other.m_type)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_tree::~a65_tree(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_tree &
a65_tree::operator=(
	__in const a65_tree &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_id::operator=(other);
		m_node = other.m_node;
		m_node_map = other.m_node_map;
		m_node_root = other.m_node_root;
		m_type = other.m_type;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

size_t
a65_tree::add_child(
	__in int type,
	__in_opt uint32_t token,
	__in_opt size_t position
	)
{
	uint32_t id;
	size_t result;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x), Position=%u", type, A65_NODE_STRING(type), token, token, position);

	if(m_node == A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("Parent node undefined", "%u(%x)", token, token);
	}

	a65_node node(type, token, m_node);

	id = node.id();
	m_node_map.insert(std::make_pair(id, node));
	result = find(m_node)->second.add_child(id, position);

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

void
a65_tree::add_root(
	__in int type,
	__in_opt uint32_t token
	)
{
	uint32_t id;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x)", type, A65_NODE_STRING(type), token, token);

	if(m_node_root != A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("Root node redefined", "%u(%x)", token, token);
	}

	a65_node node(type, token);

	id = node.id();
	m_node_map.insert(std::make_pair(id, node));
	m_node = id;
	m_node_root = id;

	A65_DEBUG_EXIT();
}

bool
a65_tree::empty(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_node_root == A65_UUID_INVALID);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

std::map<uint32_t, a65_node>::iterator
a65_tree::find(
	__in uint32_t id
	)
{
	std::map<uint32_t, a65_node>::iterator result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	result = m_node_map.find(id);
	if(result == m_node_map.end()) {
		A65_THROW_EXCEPTION_INFO("Node not found", "%u(%x)", id, id);
	}

	A65_DEBUG_EXIT_INFO("Result={%u(%x), %s}", result->first, result->first, A65_STRING_CHECK(result->second.to_string()));
	return result;
}

bool
a65_tree::has_child(
	__in size_t position
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	result = (m_node != A65_UUID_INVALID);
	if(result) {
		std::map<uint32_t, a65_node>::const_iterator entry = m_node_map.find(m_node);

		result = (entry != m_node_map.end());
		if(result) {
			result = entry->second.has_child(position);
		}
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_tree::has_parent(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_node != A65_UUID_INVALID);
	if(result) {
		std::map<uint32_t, a65_node>::const_iterator entry = m_node_map.find(m_node);

		result = (entry != m_node_map.end());
		if(result) {
			result = entry->second.has_parent();
		}
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_tree::has_root(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_node_root != A65_UUID_INVALID);
	if(result) {
		result = (m_node_map.find(m_node_root) != m_node_map.end());
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_tree::match(
	__in int type
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_TREE_STRING(type));

	result = (type == m_type);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_tree::move_child(
	__in size_t position
	)
{
	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(!has_child(position)) {
		A65_THROW_EXCEPTION_INFO("Node child position out-of-range", "%u", position);
	}

	m_node = find(m_node)->second.child(position);

	A65_DEBUG_EXIT();
}

void
a65_tree::move_child(
	__in a65_tree &tree,
	__in size_t position
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p, Position=%u", &tree, position);

	if(!tree.has_child(position)) {
		A65_THROW_EXCEPTION_INFO("No child node found at position", "%u", position);
	}

	tree.move_child(position);

	A65_DEBUG_EXIT();
}

void
a65_tree::move_parent(void)
{
	A65_DEBUG_ENTRY();

	if(!has_parent()) {
		A65_THROW_EXCEPTION_INFO("No parent node found", "%u(%x)", m_node, m_node);
	}

	m_node = find(m_node)->second.parent();

	A65_DEBUG_EXIT();
}

void
a65_tree::move_parent(
	__in a65_tree &tree
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	if(tree.has_parent()) {
		tree.move_parent();
	}

	A65_DEBUG_EXIT();
}

void
a65_tree::move_root(void)
{
	A65_DEBUG_ENTRY();

	if(!has_root()) {
		A65_THROW_EXCEPTION_INFO("No root node found", "%u(%x)", m_node, m_node);
	}

	m_node = m_node_root;

	A65_DEBUG_EXIT();
}

a65_node &
a65_tree::node(
	__in_opt uint32_t id
	)
{
	std::map<uint32_t, a65_node>::iterator result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	if(id == A65_UUID_INVALID) {
		id = m_node;
	}

	if(id == A65_UUID_INVALID) {
		A65_THROW_EXCEPTION("Tree is empty");
	}

	result = find(id);

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result->second.to_string()));
	return result->second;
}

void
a65_tree::remove(void)
{
	size_t child;
	a65_node parent;

	A65_DEBUG_ENTRY();

	parent = node();

	for(child = 0; child < parent.child_count(); ++child) {
		move_child(child);
		remove();
	}

	parent.remove_all_children();

	if(parent.has_parent()) {
		move_parent();
	} else {
		m_node = A65_UUID_INVALID;
		m_node_root = A65_UUID_INVALID;
	}

	m_node_map.erase(find(parent.id()));

	A65_DEBUG_EXIT();
}

void
a65_tree::remove_child(
	__in size_t position
	)
{
	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	move_child(position);
	remove();
	node().remove_child(position);

	A65_DEBUG_EXIT();
}

void
a65_tree::remove_root(void)
{
	A65_DEBUG_ENTRY();

	m_node = A65_UUID_INVALID;
	m_node_map.clear();
	m_node_root = A65_UUID_INVALID;

	A65_DEBUG_EXIT();
}

void
a65_tree::set(
	__in int type
	)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_TREE_STRING(type));

	m_type = type;

	A65_DEBUG_EXIT();
}

std::string
a65_tree::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result << "{" << a65_id::to_string() << "} [" << A65_TREE_STRING(m_type) << "] <" << m_node_map.size() << ">";

	if(has_root()) {
		result << " {" << A65_STRING_HEX(uint32_t, m_node_root) << ", " << A65_STRING_HEX(uint32_t, m_node) << "}";
	}

	A65_DEBUG_EXIT();
	return result.str();
}

int
a65_tree::type(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%s)", m_type, A65_TREE_STRING(m_type));
	return m_type;
}
