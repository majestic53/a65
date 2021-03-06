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

#include "../inc/a65_node.h"
#include "../inc/a65_utility.h"

a65_node::a65_node(
	__in_opt int type,
	__in_opt uint32_t token,
	__in_opt uint32_t parent,
	__in_opt const std::vector<uint32_t> &child
	) :
		m_child(child),
		m_parent(parent),
		m_token(token),
		m_type(type)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x), Parent=%u(%x), Child[%u]=%p", type, A65_NODE_STRING(type),
		token, token, parent, parent, child.size(), &child);
	A65_DEBUG_EXIT();
}

a65_node::a65_node(
	__in const a65_node &other
	) :
		a65_id(other),
		m_child(other.m_child),
		m_parent(other.m_parent),
		m_token(other.m_token),
		m_type(other.m_type)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_node::~a65_node(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_node &
a65_node::operator=(
	__in const a65_node &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_id::operator=(other);
		m_child = other.m_child;
		m_parent = other.m_parent;
		m_token = other.m_token;
		m_type = other.m_type;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

size_t
a65_node::add_child(
	__in uint32_t id,
	__in_opt size_t position
	)
{
	size_t result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x), Position=%u", id, id, position);

	result = position;
	if(result == A65_NODE_POSITION_UNDEFINED) {
		result = m_child.size();
	}

	if(result > m_child.size()) {
		A65_THROW_EXCEPTION_INFO("Node child position too large", "%u (max=%u)", result, m_child.size());
	}

	if(result < m_child.size()) {
		m_child.insert(m_child.begin() + result, id);
	} else {
		m_child.push_back(id);
	}

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

uint32_t
a65_node::child(
	__in size_t position
	) const
{
	uint32_t result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(m_child.empty()) {
		A65_THROW_EXCEPTION("Node is empty");
	} else if(position >= m_child.size()) {
		A65_THROW_EXCEPTION_INFO("Node child position out-of-range", "%u (max=%u)", position, m_child.size() - 1);
	}

	result = m_child.at(position);

	A65_DEBUG_EXIT_INFO("Result=%u(%x)", result, result);
	return result;
}

size_t
a65_node::child_count(void) const
{
	size_t result;

	A65_DEBUG_ENTRY();

	result = m_child.size();

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

bool
a65_node::has_child(
	__in size_t position
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	result = (position < m_child.size());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_node::has_parent(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_parent != A65_UUID_INVALID);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_node::has_token(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_token != A65_UUID_INVALID);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_node::is_leaf(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = m_child.empty();

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_node::is_root(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_parent == A65_UUID_INVALID);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_node::match(
	__in int type
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_NODE_STRING(type));

	result = (type == m_type);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

uint32_t
a65_node::parent(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_parent, m_parent);
	return m_parent;
}

void
a65_node::remove_all_children(void)
{
	A65_DEBUG_ENTRY();

	m_child.clear();

	A65_DEBUG_EXIT();
}

void
a65_node::remove_child(
	__in size_t position
	)
{
	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(m_child.empty()) {
		A65_THROW_EXCEPTION("Node is empty");
	} else if(position >= m_child.size()) {
		A65_THROW_EXCEPTION_INFO("Node child position out-of-range", "%u (max=%u)", position, m_child.size() - 1);
	}

	m_child.erase(m_child.begin() + position);

	A65_DEBUG_EXIT();
}

void
a65_node::set(
	__in int type
	)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_NODE_STRING(type));

	m_type = type;

	A65_DEBUG_EXIT();
}

void
a65_node::set_child(
	__in uint32_t id,
	__in size_t position
	)
{
	A65_DEBUG_ENTRY_INFO("Id=%u(%x), Position=%u", id, id, position);

	m_child.at(position) = id;

	A65_DEBUG_EXIT();
}

void
a65_node::set_parent(
	__in uint32_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	m_parent = id;

	A65_DEBUG_EXIT();
}

void
a65_node::set_token(
	__in uint32_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	m_token = id;

	A65_DEBUG_EXIT();
}

uint32_t
a65_node::token(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_token, m_token);
	return m_token;
}

std::string
a65_node::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result
#ifndef NDEBUG
		<< "{" << a65_id::to_string() << "} "
#endif // NDEBUG
		<< "[" << A65_NODE_STRING(m_type) << "] {";

	if(is_root()) {
		result << "Root";
	} else {
		result << A65_STRING_HEX(uint32_t, m_parent);
	}

	if(!is_leaf()) {
		std::vector<uint32_t>::const_iterator child;

		result << ", [" << m_child.size() << "]{";

		for(child = m_child.begin(); child != m_child.end(); ++child) {

			if(child != m_child.begin()) {
				result << ", ";
			}

			result << A65_STRING_HEX(uint32_t, *child);
		}

		result << "}";
	}

	result << "}";

	if(has_token()) {
		result << " <" << A65_STRING_HEX(uint32_t, m_token) << ">";
	}

	A65_DEBUG_EXIT();
	return result.str();
}

int
a65_node::type(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%s)", m_type, A65_NODE_STRING(m_type));
	return m_type;
}
