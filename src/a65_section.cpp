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
	__in const std::string &name,
	__in_opt uint16_t origin
	) :
		m_name(name),
		m_offset(0),
		m_origin(origin)
{
	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Origin=%u(%04x)", name.size(), A65_STRING_CHECK(name), origin, origin);

	if(name.empty()) {
		A65_THROW_EXCEPTION("Empty section name");
	}

	A65_DEBUG_EXIT();
}

a65_section::a65_section(
	__in const a65_section &other
	) :
		a65_id(other),
		m_listing(other.m_listing),
		m_name(other.m_name),
		m_offset(other.m_offset),
		m_origin(other.m_origin)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_section::~a65_section(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_section &
a65_section::operator=(
	__in const a65_section &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_id::operator=(other);
		m_listing = other.m_listing;
		m_name = other.m_name;
		m_offset = other.m_offset;
		m_origin = other.m_origin;
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

	m_listing.push_back(std::make_tuple(data, listing, m_offset));
	m_offset += data.size();

	A65_DEBUG_EXIT();
}

void
a65_section::clear(void)
{
	A65_DEBUG_ENTRY();

	m_listing.clear();
	m_offset = 0;
	m_origin = 0;

	A65_DEBUG_EXIT();
}

size_t
a65_section::count(void) const
{
	size_t result;

	A65_DEBUG_ENTRY();

	result = m_listing.size();

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

std::vector<uint8_t>
a65_section::data(
	__in size_t position
	) const
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	result = std::get<A65_SECTION_DATA>(find(position));

	A65_DEBUG_EXIT_INFO("Result[%u]=%p", result.size(), &result);
	return result;
}

bool
a65_section::empty(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = m_listing.empty();

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

std::tuple<std::vector<uint8_t>, uint16_t, uint32_t>
a65_section::find(
	__in size_t position
	) const
{
	std::tuple<std::vector<uint8_t>, uint16_t, uint32_t> result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(m_listing.empty()) {
		A65_THROW_EXCEPTION("Section is empty");
	} else if(position >= m_listing.size()) {
		A65_THROW_EXCEPTION_INFO("Section position out-of-range", "%u (max=%u)", position, m_listing.size() - 1);
	}

	result = m_listing.at(position);

	A65_DEBUG_EXIT_INFO("Result={[%u]=%p, %u(%x), %u(%04x)}",
		std::get<A65_SECTION_DATA>(result).size(), &std::get<A65_SECTION_DATA>(result),
		std::get<A65_SECTION_LISTING>(result), std::get<A65_SECTION_LISTING>(result),
		std::get<A65_SECTION_OFFSET>(result), std::get<A65_SECTION_OFFSET>(result));

	return result;
}

uint32_t
a65_section::listing(
	__in size_t position
	) const
{
	uint32_t result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	result = std::get<A65_SECTION_LISTING>(find(position));

	A65_DEBUG_EXIT_INFO("Result=%u(%x)", result, result);
	return result;
}

std::string
a65_section::name(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", m_name.size(), A65_STRING_CHECK(m_name));
	return m_name;
}

uint16_t
a65_section::offset(
	__in size_t position
	) const
{
	uint16_t result;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	result = std::get<A65_SECTION_OFFSET>(find(position));

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

uint16_t
a65_section::origin(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", m_origin, m_origin);
	return m_origin;
}

void
a65_section::set_name(
	__in const std::string &name
	)
{
	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	if(name.empty()) {
		A65_THROW_EXCEPTION("Empty section name");
	}

	m_name = name;

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

uint16_t
a65_section::size(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u", m_offset);
	return m_offset;
}

std::string
a65_section::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result
#ifndef NDEBUG
		<< "{" << a65_id::to_string() << "} "
#endif // NDEBUG
		<< "[" << A65_STRING_CHECK(m_name) << "@" << A65_STRING_HEX(uint16_t, m_origin)
		<< "] <" << m_listing.size() << ">";

	if(!m_listing.empty()) {
		result << " {" << A65_FLOAT_PREC(2, m_offset / A65_SECTION_KB_LENGTH) << " KB (" << m_offset << " bytes)}";
	}

	A65_DEBUG_EXIT();
	return result.str();
}
