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

#include "../inc/a65_id.h"
#include "../inc/a65_utility.h"

a65_id::a65_id(void) :
	m_id(A65_UUID_INVALID)
{
	A65_DEBUG_ENTRY();

	generate();

	A65_DEBUG_EXIT();
}

a65_id::a65_id(
	__in const a65_id &other
	) :
		m_id(other.m_id)
{
	A65_DEBUG_ENTRY();

	increment();

	A65_DEBUG_EXIT();
}

a65_id::~a65_id(void)
{
	A65_DEBUG_ENTRY();

	decrement();

	A65_DEBUG_EXIT();
}

a65_id &
a65_id::operator=(
	__in const a65_id &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		decrement();
		m_id = other.m_id;
		increment();
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_id::decrement(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.decrement(m_id);
	}

	m_id = A65_UUID_INVALID;

	A65_DEBUG_EXIT();
}

void
a65_id::generate(void)
{
	A65_DEBUG_ENTRY();

	m_id = a65_uuid::instance().generate();

	A65_DEBUG_EXIT();
}

uint32_t
a65_id::id(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_id, m_id);
	return m_id;
}

void
a65_id::increment(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.increment(m_id);
	}

	A65_DEBUG_EXIT();
}

std::string
a65_id::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	if(m_id != A65_UUID_INVALID) {
		result << A65_STRING_HEX(uint32_t, m_id);
	} else {
		result << A65_ID_UNDEFINED;
	}

	A65_DEBUG_EXIT();
	return result.str();
}
