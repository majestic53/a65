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

#include <cstring>
#include "../inc/a65_object.h"
#include "../inc/a65_utility.h"
#include "../inc/a65_uuid.h"

a65_object::a65_object(
	__in_opt const std::map<uint16_t, a65_section> &section
	) :
		m_header({}),
		m_payload(nullptr),
		m_payload_size(0)
{
	A65_DEBUG_ENTRY_INFO("Section[%u]=%p", section.size(), &section);

	if(!section.empty()) {
		import(section);
	} else {
		clear();
	}

	A65_DEBUG_EXIT();
}

a65_object::a65_object(
	__in const std::vector<uint8_t> &data
	) :
		m_header({}),
		m_payload(nullptr),
		m_payload_size(0)
{
	A65_DEBUG_ENTRY_INFO("Data[%u]=%p", data.size(), &data);

	if(!data.empty()) {
		import(data);
	} else {
		clear();
	}

	A65_DEBUG_EXIT();
}

a65_object::a65_object(
	__in const std::string &path
	) :
		m_header({}),
		m_payload(nullptr),
		m_payload_size(0)
{
	A65_DEBUG_ENTRY_INFO("Path[%u]=%s", path.size(), A65_STRING_CHECK(path));

	read(path);

	A65_DEBUG_EXIT();
}

a65_object::a65_object(
	__in const a65_object &other
	) :
		a65_id(other),
		m_header({}),
		m_payload(nullptr),
		m_payload_size(0)
{
	A65_DEBUG_ENTRY();

	copy(other);

	A65_DEBUG_EXIT();
}

a65_object::~a65_object(void)
{
	A65_DEBUG_ENTRY();

	clear();

	A65_DEBUG_EXIT();
}

a65_object &
a65_object::operator=(
	__in const a65_object &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_id::operator=(other);
		copy(other);
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

std::vector<uint8_t>
a65_object::as_data(
	__in_opt bool header
	) const
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Header=%x", header);

	if(header) {
		result.insert(result.end(), (char *)&m_header, ((char *)&m_header) + sizeof(m_header));
	}

	if(m_payload) {
		result.insert(result.end(), (char *)m_payload, ((char *)m_payload) + m_payload_size);
	}

	A65_DEBUG_EXIT();
	return result;
}

void
a65_object::clear(void)
{
	A65_DEBUG_ENTRY();

	std::memset(&m_header, 0, sizeof(m_header));
	m_header.metadata.major = A65_VERSION_MAJOR;
	m_header.metadata.minor = A65_VERSION_MINOR;
	m_header.metadata.type = A65_OBJECT_TYPE;
	m_header.magic = A65_OBJECT_MAGIC;

	if(m_payload) {
		delete [] m_payload;
		m_payload = nullptr;
	}

	m_payload_size = 0;
	m_section.clear();

	A65_DEBUG_EXIT();
}

bool
a65_object::contains_section(
	__in const std::string &name
	) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_section.find(name) != m_section.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_object::contains_section(
	__in size_t position
	) const
{
	bool result = false;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(m_payload) {
		result = (position < m_payload->metadata.count);
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_object::copy(
	__in const a65_object &other
	)
{
	A65_DEBUG_ENTRY();

	clear();

	m_payload_size = other.m_payload_size;
	if(m_payload_size) {

		m_payload = (a65_object_payload_t *) new uint8_t[m_payload_size];
		if(!m_payload) {
			A65_THROW_EXCEPTION("Object payload allocation failed");
		}

		std::memcpy(m_payload, other.m_payload, m_payload_size);
	}

	std::memcpy(&m_header, &other.m_header, sizeof(m_header));
	m_section = other.m_section;

	A65_DEBUG_EXIT();
}

size_t
a65_object::count(void) const
{
	size_t result = 0;

	A65_DEBUG_ENTRY();

	if(m_payload) {
		result = m_payload->metadata.count;
	}

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

bool
a65_object::empty(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_payload == nullptr);
	if(!result) {
		result = (m_payload->metadata.count == 0);
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

std::map<std::string, uint32_t>::const_iterator
a65_object::find(
	__in const std::string &name
	) const
{
	std::map<std::string, uint32_t>::const_iterator result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	result = m_section.find(name);
	if(result == m_section.end()) {
		A65_THROW_EXCEPTION_INFO("Object does not contain section", "[%u]%s", name.size(), A65_STRING_CHECK(name));
	}

	A65_DEBUG_EXIT_INFO("Result={[%u]%s, %u(%x)}", result->first.size(), A65_STRING_CHECK(result->first), result->second, result->second);
	return result;
}

void
a65_object::import(
	__in const std::map<uint16_t, a65_section> &section
	)
{
	uint32_t count = 0, offset;
	std::map<uint16_t, a65_section>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Section[%u]=%p", section.size(), &section);

	clear();

	m_payload_size = sizeof(a65_object_payload_t);
	offset = m_payload_size;

	if(!section.empty()) {
		m_payload_size += (section.size() * sizeof(a65_object_section_t));
		offset = m_payload_size;

		for(entry = section.begin(); entry != section.end(); ++entry) {
			m_payload_size += entry->second.size();
		}
	}

	m_payload = (a65_object_payload_t *) new uint8_t[m_payload_size];
	if(!m_payload) {
		A65_THROW_EXCEPTION("Object payload allocation failed");
	}

	std::memset(m_payload, 0, m_payload_size);
	m_payload->metadata.count = section.size();
	m_payload->metadata.size = m_payload_size;

	for(entry = section.begin(); entry != section.end(); ++count, ++entry) {
		uint32_t size;
		std::vector<uint8_t> data;
		std::string name = entry->second.name();

		if(m_section.find(name) != m_section.end()) {
			A65_THROW_EXCEPTION_INFO("Duplicate object payload section", "%u, [%u]%s", count, name.size(), A65_STRING_CHECK(name));
		}

		m_section.insert(std::make_pair(name, count));

		for(size_t iter = 0; iter < entry->second.count(); ++iter) {

			std::vector<uint8_t> subdata = entry->second.data(iter);
			if(subdata.empty()) {
				A65_THROW_EXCEPTION_INFO("Object payload section empty", "%u", iter);
			}

			data.insert(data.end(), subdata.begin(), subdata.end());
		}

		size = data.size();

		m_payload->section[count].origin = entry->first;
		m_payload->section[count].offset = offset;
		m_payload->section[count].size = size;

		std::memcpy(&m_payload->section[count].name, &name[0], std::min(name.size(), (size_t) (A65_OBJECT_SECTION_NAME_MAX - 1)));

		if(name.size() > (A65_OBJECT_SECTION_NAME_MAX - 1)) {
			A65_DEBUG_MESSAGE_INFO(A65_DEBUG_LEVEL_WARNING, "Object section name truncated", "%s", m_payload->section[count].name);
		}

		std::memcpy(&((char *)m_payload)[offset], &data[0], size);
		offset += size;
	}

	A65_DEBUG_EXIT();
}

void
a65_object::import(
	__in const std::vector<uint8_t> &data
	)
{
	size_t length;
	a65_object_header_t *header;
	a65_object_payload_t *payload;

	A65_DEBUG_ENTRY_INFO("Data[%u]=%p", data.size(), &data);

	length = data.size();
	if(length < sizeof(a65_object_header_t)) {
		A65_THROW_EXCEPTION_INFO("Invalid object length", "%u (min=%u)", length, sizeof(a65_object_header_t));
	}

	header = (a65_object_header_t *)&data[0];
	if(!header) {
		A65_THROW_EXCEPTION("Malformed object header");
	}

	if(header->magic != A65_OBJECT_MAGIC) {
		A65_THROW_EXCEPTION_INFO("Object header mismatch", "Magic=%u(%08x) (expecting=%u(%08x))", header->magic, header->magic,
			A65_OBJECT_MAGIC, A65_OBJECT_MAGIC);
	} else if((header->metadata.major != A65_VERSION_MAJOR) || (header->metadata.minor != A65_VERSION_MINOR)) {
		A65_THROW_EXCEPTION_INFO("Object header mismatch", "Version=%u.%u (expecting=%u.%u)", header->metadata.major, header->metadata.minor,
			A65_VERSION_MAJOR, A65_VERSION_MINOR);
	} else if(header->metadata.type != A65_OBJECT_TYPE) {
		A65_THROW_EXCEPTION_INFO("Object header mismatch", "Type=%u(%04x) (expecting=%u(%04x))", header->metadata.type, header->metadata.type,
			A65_OBJECT_TYPE, A65_OBJECT_TYPE);
	}

	clear();

	if(length > sizeof(a65_object_header_t)) {
		uint32_t size;

		payload = (a65_object_payload_t *)&data[sizeof(a65_object_header_t)];
		if(!payload) {
			A65_THROW_EXCEPTION("Malformed object payload");
		}

		if(payload->metadata.size < sizeof(a65_object_payload_t)) {
			A65_THROW_EXCEPTION_INFO("Object payload length mismatch", "%u (min=%u)", payload->metadata.size, sizeof(a65_object_payload_t));
		}

		size = payload->metadata.size;

		for(uint32_t entry = 0; entry < payload->metadata.count; ++entry) {
			std::string name;

			a65_object_section_t *section = (a65_object_section_t *)&payload->section[entry];
			if(!section) {
				A65_THROW_EXCEPTION_INFO("Malformed object payload section", "%u", entry);
			}

			if((section->offset > size) || ((section->offset + section->size) > size)) {
				A65_THROW_EXCEPTION_INFO("Object payload section out-of-bounds", "%u (%u - %u))", entry, section->offset,
					section->offset + section->size);
			}

			name = section->name;
			if(name.empty()) {
				A65_THROW_EXCEPTION_INFO("Malformed object payload section name (cannot be empty)", "%u", entry);
			}

			if(m_section.find(name) != m_section.end()) {
				A65_THROW_EXCEPTION_INFO("Duplicate object payload section", "%u, [%u]%s", entry, name.size(), A65_STRING_CHECK(name));
			}

			m_section.insert(std::make_pair(name, entry));
		}

		m_payload = (a65_object_payload_t *) new uint8_t[size];
		if(!m_payload) {
			A65_THROW_EXCEPTION("Object payload allocation failed");
		}

		std::memcpy(m_payload, payload, size);
		m_payload_size = size;
	}

	std::memcpy(&m_header, header, sizeof(m_header));

	A65_DEBUG_EXIT();
}

void
a65_object::read(
	__in const std::string &path
	)
{
	size_t length;
	std::vector<uint8_t> data;

	A65_DEBUG_ENTRY_INFO("Path[%u]=%s", path.size(), A65_STRING_CHECK(path));

	length = a65_utility::read_file(path, data);
	if(length != data.size()) {
		A65_THROW_EXCEPTION_INFO("File length mismatch", "%u (expecting=%u)", data.size(), length);
	}

	import(data);

	A65_DEBUG_EXIT();
}

uint16_t
a65_object::section(
	__in const std::string &name,
	__inout std::vector<uint8_t> &data
	) const
{
	uint16_t result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Data=%p", name.size(), A65_STRING_CHECK(name), &data);

	result = section(find(name)->second, data);

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

uint16_t
a65_object::section(
	__in size_t position,
	__inout std::vector<uint8_t> &data
	) const
{
	uint16_t result;
	a65_object_section_t *section;

	A65_DEBUG_ENTRY_INFO("Position=%u, Data=%p", position, &data);

	if(!contains_section(position)) {
		A65_THROW_EXCEPTION_INFO("Object does not contain section", "%u", position);
	}

	section = &m_payload->section[position];
	if(!section) {
		A65_THROW_EXCEPTION_INFO("Malformed object payload section", "%u", position);
	}

	data = std::vector<uint8_t>(((char *)m_payload) + section->offset, ((char *)m_payload) + section->offset + section->size);

	result = section->origin;

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

size_t
a65_object::size(
	__in_opt bool header
	) const
{
	size_t result;

	A65_DEBUG_ENTRY_INFO("Header=%x", header);

	result = m_payload_size;

	if(header) {
		result += sizeof(a65_object_header_t);
	}

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

std::string
a65_object::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	if(m_payload) {

		for(uint32_t entry = 0; entry < m_payload->metadata.count; ++entry) {

			a65_object_section_t *section = &m_payload->section[entry];
			if(!section) {
				A65_THROW_EXCEPTION_INFO("Malformed object payload section", "%u", entry);
			}

			std::vector<uint8_t> data = std::vector<uint8_t>(&((char *)m_payload)[section->offset],
				&((char *)m_payload)[section->offset] + section->size);

			result << std::endl
#ifndef NDEBUG
					<< "{" << a65_id::to_string() << "} "
#endif // NDEBUG
					<< "[" << section->name
					<< "@" << A65_STRING_HEX(uint16_t, section->origin) << "]"
				<< " {" << A65_STRING_HEX(uint32_t, section->offset) << ", " << A65_STRING_HEX(uint32_t, section->size) << "}";

			if(!data.empty()) {
				result << std::endl << a65_utility::data_as_string(data, section->origin);
			}
		}
	}

	A65_DEBUG_EXIT();

	return result.str();
}

void
a65_object::write(
	__in const std::string &path
	) const
{
	A65_DEBUG_ENTRY_INFO("Path[%u]=%s", path.size(), A65_STRING_CHECK(path));

	a65_utility::write_file(path, as_data());

	A65_DEBUG_EXIT();
}
