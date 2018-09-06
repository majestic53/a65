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

#include "../inc/a65_utility.h"
#include "../inc/a65_uuid.h"

#define A65_UUID_REFERENCE_START 1

a65_uuid::a65_uuid(void) :
	m_next(A65_UUID_INVALID + 1)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_uuid::~a65_uuid(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

bool
a65_uuid::contains(
	__in a65_uuid_t id
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	if(id == A65_UUID_INVALID) {
		A65_THROW_EXCEPTION_INFO("Invalid uuid", "%u(%x)", id, id);
	}

	result = (m_reference.find(id) != m_reference.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_uuid::decrement(
	__in a65_uuid_t id
	)
{
	std::map<a65_uuid_t, size_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	entry = find(id);
	if(entry->second <= A65_UUID_REFERENCE_START) {
		m_surplus.insert(entry->first);
		m_reference.erase(entry);
	} else {
		--entry->second;
	}

	A65_DEBUG_EXIT();
}

std::map<a65_uuid_t, size_t>::iterator
a65_uuid::find(
	__in a65_uuid_t id
	)
{
	std::map<a65_uuid_t, size_t>::iterator result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	result = m_reference.find(id);
	if(result == m_reference.end()) {
		A65_THROW_EXCEPTION_INFO("Uuid not found", "%u(%x)", id, id);
	}

	A65_DEBUG_EXIT_INFO("Result={%u(%x), %u}", result->first, result->first, result->second);
	return result;
}

a65_uuid_t
a65_uuid::generate(void)
{
	a65_uuid_t result = A65_UUID_INVALID;

	A65_DEBUG_ENTRY();

	if(!m_surplus.empty()) {
		std::set<a65_uuid_t>::iterator entry = m_surplus.begin();

		result = *entry;
		m_reference.insert(std::make_pair(result, A65_UUID_REFERENCE_START));
		m_surplus.erase(entry);
	} else if(m_next != A65_UUID_INVALID) {
		result = m_next++;
	} else {
		A65_THROW_EXCEPTION("No avaliable uuid");
	}

	A65_DEBUG_EXIT_INFO("Result=%u(%x)", result, result);
	return result;
}

void
a65_uuid::increment(
	__in a65_uuid_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	++find(id)->second;

	A65_DEBUG_EXIT();
}

a65_uuid &
a65_uuid::instance(void)
{
	A65_DEBUG_ENTRY();

	static a65_uuid result;

	A65_DEBUG_EXIT_INFO("Result=%p", &result);
	return result;
}
