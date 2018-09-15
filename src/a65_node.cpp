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

#include <sstream>
#include "../inc/a65_node.h"
#include "../inc/a65_utility.h"

a65_node::a65_node(
	__in_opt int type,
	__in_opt uint32_t token,
	__in_opt uint32_t parent,
	__in_opt uint32_t child_left,
	__in_opt uint32_t child_right
	) :
		m_child_left(child_left),
		m_child_right(child_right),
		m_id(A65_UUID_INVALID),
		m_parent(parent),
		m_token(token),
		m_type(type)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Token=%u(%x), Parent=%u(%x), Child={%u(%x), %u(%x)}", type, A65_NODE_STRING(type),
		token, token, parent, parent, child_left, child_left, child_right, child_right);

	generate();

	A65_DEBUG_EXIT();
}

a65_node::a65_node(
	__in const a65_node &other
	) :
		m_child_left(other.m_child_left),
		m_child_right(other.m_child_right),
		m_id(other.m_id),
		m_parent(other.m_parent),
		m_token(other.m_token),
		m_type(other.m_type)
{
	A65_DEBUG_ENTRY();

	increment();

	A65_DEBUG_EXIT();
}

a65_node::~a65_node(void)
{
	A65_DEBUG_ENTRY();

	decrement();

	A65_DEBUG_EXIT();
}

a65_node &
a65_node::operator=(
	__in const a65_node &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		decrement();
		m_child_left = other.m_child_left;
		m_child_right = other.m_child_right;
		m_id = other.m_id;
		m_parent = other.m_parent;
		m_token = other.m_token;
		m_type = other.m_type;
		increment();
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

uint32_t
a65_node::child_left(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_child_left, m_child_left);
	return m_child_left;
}

uint32_t
a65_node::child_right(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_child_right, m_child_right);
	return m_child_right;
}

void
a65_node::decrement(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.decrement(m_id);
		m_id = A65_UUID_INVALID;
	}

	A65_DEBUG_EXIT();
}

void
a65_node::generate(void)
{
	A65_DEBUG_ENTRY();

	m_id = a65_uuid::instance().generate();

	A65_DEBUG_EXIT();
}

bool
a65_node::has_child_left(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_child_left != A65_UUID_INVALID);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_node::has_child_right(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_child_right != A65_UUID_INVALID);

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

uint32_t
a65_node::id(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_id, m_id);
	return m_id;
}

void
a65_node::increment(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.increment(m_id);
	}

	A65_DEBUG_EXIT();
}

bool
a65_node::is_leaf(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = ((m_child_left == A65_UUID_INVALID)
			&& (m_child_right == A65_UUID_INVALID));

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
a65_node::set(
	__in int type
	)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s)", type, A65_NODE_STRING(type));

	m_type = type;

	A65_DEBUG_EXIT();
}

void
a65_node::set_child_left(
	__in uint32_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	m_child_left = id;

	A65_DEBUG_EXIT();
}

void
a65_node::set_child_right(
	__in uint32_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	m_child_right = id;

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

	result << "{" << A65_STRING_HEX(uint32_t, m_id) << "} [" << A65_NODE_STRING(m_type) << "] {";

	if(is_root()) {
		result << "Root";
	} else {
		result << A65_STRING_HEX(uint32_t, m_parent);
	}

	if(!is_leaf()) {
		result << ", {" << A65_STRING_HEX(uint32_t, m_child_left) << ", " << A65_STRING_HEX(uint32_t, m_child_right) << "}";
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
