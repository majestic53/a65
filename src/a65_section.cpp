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

#include "../inc/a65_section.h"
#include "../inc/a65_utility.h"

a65_section::a65_section(
	__in_opt uint16_t origin
	) :
		m_id(A65_UUID_INVALID),
		m_offset_position(0),
		m_origin(origin)
{
	A65_DEBUG_ENTRY_INFO("Origin=%u(%04x)", origin, origin);

	generate();

	A65_DEBUG_EXIT();
}

a65_section::a65_section(
	__in const a65_section &other
	) :
		m_data(other.m_data),
		m_id(other.m_id),
		m_listing(other.m_listing),
		m_offset(other.m_offset),
		m_offset_position(other.m_offset_position),
		m_origin(other.m_origin)
{
	A65_DEBUG_ENTRY();

	increment();

	A65_DEBUG_EXIT();
}

a65_section::~a65_section(void)
{
	A65_DEBUG_ENTRY();

	decrement();

	A65_DEBUG_EXIT();
}

a65_section &
a65_section::operator=(
	__in const a65_section &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		decrement();
		m_data = other.m_data;
		m_id = other.m_id;
		m_listing = other.m_listing;
		m_offset = other.m_offset;
		m_offset_position = other.m_offset_position;
		m_origin = other.m_origin;
		increment();
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_section::add(
	__in const std::vector<uint8_t> &data,
	__in uint32_t listing
	)
{
	A65_DEBUG_ENTRY_INFO("Data[%u]=%p, Listing=%u(%x)", data.size(), &data, listing, listing);

	m_listing.push_back(listing);
	m_offset.push_back(std::make_pair(m_data.size(), data.size()));
	m_data.insert(m_data.end(), data.begin(), data.end());

	A65_DEBUG_EXIT();
}

void
a65_section::clear(void)
{
	A65_DEBUG_ENTRY();

	m_data.clear();
	m_listing.clear();
	m_offset.clear();
	m_offset_position = 0;
	m_origin = 0;

	A65_DEBUG_EXIT();
}

std::vector<uint8_t>
a65_section::data(
	__in_opt size_t position
	) const
{
	size_t begin, end;
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(position == A65_SECTION_POSITION_UNDEFINED) {
		position = m_offset_position;
	}

	if(position >= m_offset.size()) {
		A65_THROW_EXCEPTION_INFO("Section position out-of-range", "%u (max=%u)", position, m_offset.size());
	}

	const std::pair<uint16_t, uint16_t> &offset = m_offset.at(position);
	begin = offset.first;
	end = (begin + offset.second);

	if((begin >= m_data.size()) || (end >= m_data.size())) {
		A65_THROW_EXCEPTION_INFO("Section data out-of-range", "[%u-%u] (max=%u)", begin, end, m_data.size());
	}

	result = std::vector<uint8_t>(m_data.begin() + begin, m_data.begin() + end);

	A65_DEBUG_EXIT_INFO("Result[%u]=%p", result.size(), &result);
	return result;
}

void
a65_section::decrement(void)
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
a65_section::empty(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = m_data.empty();

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_section::generate(void)
{
	A65_DEBUG_ENTRY();

	m_id = a65_uuid::instance().generate();

	A65_DEBUG_EXIT();
}

bool
a65_section::has_next(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_offset_position < m_offset.size());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_section::has_previous(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_offset_position > 0);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

uint32_t
a65_section::id(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Id=%u(%x)", m_id, m_id);
	return m_id;
}

void
a65_section::increment(void)
{
	A65_DEBUG_ENTRY();

	a65_uuid &instance = a65_uuid::instance();
	if(instance.contains(m_id)) {
		instance.increment(m_id);
	}

	A65_DEBUG_EXIT();
}

uint32_t
a65_section::listing(
	__in_opt size_t position
	) const
{
	uint32_t result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(position == A65_SECTION_POSITION_UNDEFINED) {
		position = m_offset_position;
	}

	if(position >= m_listing.size()) {
		A65_THROW_EXCEPTION_INFO("Section position out-of-range", "%u (max=%u)", position, m_listing.size());
	}

	result = m_listing.at(position);

	A65_DEBUG_EXIT_INFO("Result=%u(%x)", result, result);
	return result;
}

void
a65_section::move_next(void)
{
	A65_DEBUG_ENTRY();

	if(!has_next()) {
		A65_THROW_EXCEPTION("No next offset in section");
	}

	++m_offset_position;

	A65_DEBUG_EXIT();
}

void
a65_section::move_previous(void)
{
	A65_DEBUG_ENTRY();

	if(!has_previous()) {
		A65_THROW_EXCEPTION("No previous offset in section");
	}

	--m_offset_position;

	A65_DEBUG_EXIT();
}

uint16_t
a65_section::origin(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Id=%u(%04x)", m_origin, m_origin);
	return m_origin;
}

void
a65_section::reset(void)
{
	A65_DEBUG_ENTRY();

	m_offset_position = 0;

	A65_DEBUG_EXIT();
}

void
a65_section::set_origin(
	__in uint16_t origin
	)
{
	A65_DEBUG_ENTRY_INFO("Origin=%u(%04x)", origin, origin);

	m_origin = origin;

	A65_DEBUG_EXIT();
}

size_t
a65_section::size(void) const
{
	size_t result;

	A65_DEBUG_ENTRY();

	result = m_data.size();

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

std::string
a65_section::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result << "{" << A65_STRING_HEX(uint32_t, m_id) << "} [" << A65_STRING_HEX(uint16_t, m_origin) << "] <" << m_offset.size() << ">";

	if(!empty()) {
		result << " {" << A65_FLOAT_PREC(2, m_data.size() / A65_SECTION_KB_LENGTH) << " KB (" << m_data.size() << " bytes)}";
	}

	A65_DEBUG_EXIT();
	return result.str();
}
