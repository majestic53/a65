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
		m_id(A65_UUID_INVALID),
		m_node(A65_UUID_INVALID),
		m_node_root(A65_UUID_INVALID),
		m_type(type)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_TREE_STRING(type));

	generate();

	A65_DEBUG_EXIT();
}

a65_tree::a65_tree(
	__in const a65_tree &other
	) :
		m_id(other.m_id),
		m_node(other.m_node),
		m_node_map(other.m_node_map),
		m_node_root(other.m_node_root),
		m_type(other.m_type)
{
	A65_DEBUG_ENTRY();

	increment();

	A65_DEBUG_EXIT();
}

a65_tree::~a65_tree(void)
{
	A65_DEBUG_ENTRY();

	decrement();

	A65_DEBUG_EXIT();
}

a65_tree &
a65_tree::operator=(
	__in const a65_tree &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		decrement();
		m_id = other.m_id;
		m_node = other.m_node;
		m_node_map = other.m_node_map;
		m_node_root = other.m_node_root;
		m_type = other.m_type;
		increment();
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_tree::add(
	__in int type,
	__in uint32_t token
	)
{
	uint32_t id;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x)", type, A65_NODE_STRING(type), token, token);

	if(m_node_root != A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("Node redefined", "%u(%x)", token, token);
	}

	a65_node node(type, token);

	id = node.id();
	m_node_map.insert(std::make_pair(id, node));
	m_node = id;
	m_node_root = id;

	A65_DEBUG_EXIT();
}

void
a65_tree::add_child_left(
	__in int type,
	__in uint32_t token
	)
{
	uint32_t id;
	std::map<uint32_t, a65_node>::iterator parent;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x)", type, A65_NODE_STRING(type), token, token);

	if(m_node == A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("Parent node undefined", "%u(%x)", token, token);
	}

	parent = find(m_node);
	if(parent->second.has_child_left()) {
		A65_THROW_EXCEPTION_INFO("Left child node redefined", "%u(%x)", token, token);
	}

	a65_node node(type, token, m_node);

	id = node.id();
	m_node_map.insert(std::make_pair(id, node));
	parent->second.set_child_left(id);

	A65_DEBUG_EXIT();
}

void
a65_tree::add_child_right(
	__in int type,
	__in uint32_t token
	)
{
	uint32_t id;
	std::map<uint32_t, a65_node>::iterator parent;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x)", type, A65_NODE_STRING(type), token, token);

	if(m_node == A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("Parent node undefined", "%u(%x)", token, token);
	}

	parent = find(m_node);
	if(parent->second.has_child_right()) {
		A65_THROW_EXCEPTION_INFO("Right child node redefined", "%u(%x)", token, token);
	}

	a65_node node(type, token, m_node);

	id = node.id();
	m_node_map.insert(std::make_pair(id, node));
	parent->second.set_child_right(id);

	A65_DEBUG_EXIT();
}

void
a65_tree::decrement(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.decrement(m_id);
		m_id = A65_UUID_INVALID;
	}

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

void
a65_tree::generate(void)
{
	A65_DEBUG_ENTRY();

	m_id = a65_uuid::instance().generate();

	A65_DEBUG_EXIT();
}

bool
a65_tree::has_child_left(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_node != A65_UUID_INVALID);
	if(result) {
		std::map<uint32_t, a65_node>::const_iterator entry = m_node_map.find(m_node);

		result = (entry != m_node_map.end());
		if(result) {
			result = entry->second.has_child_left();
		}
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_tree::has_child_right(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_node != A65_UUID_INVALID);
	if(result) {
		std::map<uint32_t, a65_node>::const_iterator entry = m_node_map.find(m_node);

		result = (entry != m_node_map.end());
		if(result) {
			result = entry->second.has_child_right();
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

uint32_t
a65_tree::id(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_id, m_id);
	return m_id;
}

void
a65_tree::increment(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.increment(m_id);
	}

	A65_DEBUG_EXIT();
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
a65_tree::move_child_left(void)
{
	A65_DEBUG_ENTRY();

	if(!has_child_left()) {
		A65_THROW_EXCEPTION_INFO("No left child node found", "%u(%x)", m_node, m_node);
	}

	m_node = find(m_node)->second.child_left();

	A65_DEBUG_EXIT();
}

void
a65_tree::move_child_right(void)
{
	A65_DEBUG_ENTRY();

	if(!has_child_right()) {
		A65_THROW_EXCEPTION_INFO("No right child node found", "%u(%x)", m_node, m_node);
	}

	m_node = find(m_node)->second.child_right();

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

	result = find(id);

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result->second.to_string()));
	return result->second;
}

void
a65_tree::remove(void)
{
	uint32_t child;

	A65_DEBUG_ENTRY();

	if(m_node == A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("No node found", "%u(%x)", m_node, m_node);
	}

	if(has_child_left()) {
		move_child_left();
		remove();
	}

	if(has_child_right()) {
		move_child_right();
		remove();
	}

	child = m_node;

	if(has_parent()) {
		std::map<uint32_t, a65_node>::iterator parent;

		move_parent();
		parent = find(m_node);

		if(child == parent->second.child_left()) {
			parent->second.set_child_left(A65_UUID_INVALID);
		} else if(child == parent->second.child_right()) {
			parent->second.set_child_right(A65_UUID_INVALID);
		}
	} else {
		m_node = A65_UUID_INVALID;
		m_node_root = A65_UUID_INVALID;
	}

	m_node_map.erase(find(child));

	A65_DEBUG_EXIT();
}

void
a65_tree::remove_child_left(void)
{
	A65_DEBUG_ENTRY();

	move_child_left();
	remove();

	A65_DEBUG_EXIT();
}

void
a65_tree::remove_child_right(void)
{
	A65_DEBUG_ENTRY();

	move_child_right();
	remove();

	A65_DEBUG_EXIT();
}

std::string
a65_tree::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result << "{" << A65_STRING_HEX(uint32_t, m_id) << "} [" << A65_TREE_STRING(m_type) << "] <" << m_node_map.size() << ">";

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