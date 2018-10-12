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
#include "../inc/a65_archive.h"
#include "../inc/a65_utility.h"

a65_archive::a65_archive(
	__in_opt const std::vector<a65_object> &object
	) :
		m_header({}),
		m_payload(nullptr),
		m_payload_size(0)
{
	A65_DEBUG_ENTRY_INFO("Object[%u]=%p", object.size(), &object);

	if(!object.empty()) {
		import(object);
	} else {
		clear();
	}

	A65_DEBUG_EXIT();
}

a65_archive::a65_archive(
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

a65_archive::a65_archive(
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

a65_archive::a65_archive(
	__in const a65_archive &other
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

a65_archive::~a65_archive(void)
{
	A65_DEBUG_ENTRY();

	clear();

	A65_DEBUG_EXIT();
}

a65_archive &
a65_archive::operator=(
	__in const a65_archive &other
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
a65_archive::as_data(void) const
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY();

	result.insert(result.end(), (char *)&m_header, ((char *)&m_header) + sizeof(m_header));

	if(m_payload) {
		result.insert(result.end(), (char *)m_payload, ((char *)m_payload) + m_payload_size);
	}

	A65_DEBUG_EXIT();
	return result;
}

void
a65_archive::clear(void)
{
	A65_DEBUG_ENTRY();

	std::memset(&m_header, 0, sizeof(m_header));
	m_header.magic = A65_OBJECT_MAGIC;
	m_header.metadata.major = A65_VERSION_MAJOR;
	m_header.metadata.minor = A65_VERSION_MINOR;
	m_header.metadata.type = A65_ARCHIVE_TYPE;

	if(m_payload) {
		delete [] m_payload;
		m_payload = nullptr;
	}

	m_payload_size = 0;

	A65_DEBUG_EXIT();
}

bool
a65_archive::contains_object(
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
a65_archive::copy(
	__in const a65_archive &other
	)
{
	A65_DEBUG_ENTRY();

	clear();

	m_payload_size = other.m_payload_size;
	if(m_payload_size) {

		m_payload = (a65_archive_payload_t *) new uint8_t[m_payload_size];
		if(!m_payload) {
			A65_THROW_EXCEPTION("Archive payload allocation failed");
		}

		std::memcpy(m_payload, other.m_payload, m_payload_size);
	}

	std::memcpy(&m_header, &other.m_header, sizeof(m_header));

	A65_DEBUG_EXIT();
}

size_t
a65_archive::count(void) const
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
a65_archive::empty(void) const
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

void
a65_archive::import(
	__in const std::vector<a65_object> &object
	)
{
	uint32_t count = 0, offset;
	std::vector<a65_object>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Object[%u]=%p", object.size(), &object);

	clear();

	m_payload_size = sizeof(a65_archive_payload_t);
	offset = m_payload_size;

	if(!object.empty()) {
		m_payload_size += (object.size() * sizeof(a65_archive_object_t));
		offset = m_payload_size;

		for(entry = object.begin(); entry != object.end(); ++entry) {
			m_payload_size += entry->size();
		}
	}

	m_payload = (a65_archive_payload_t *) new uint8_t[m_payload_size];
	if(!m_payload) {
		A65_THROW_EXCEPTION("Archive payload allocation failed");
	}

	std::memset(m_payload, 0, m_payload_size);
	m_payload->metadata.count = object.size();
	m_payload->metadata.size = m_payload_size;

	for(entry = object.begin(); entry != object.end(); ++count, ++entry) {
		uint32_t size;
		std::vector<uint8_t> data = entry->as_data();

		size = data.size();
		m_payload->object[count].offset = offset;
		m_payload->object[count].size = size;
		memcpy(&((char *)m_payload)[offset], &data[0], size);
		offset += size;
	}

	A65_DEBUG_EXIT();
}

void
a65_archive::import(
	__in const std::vector<uint8_t> &data
	)
{
	size_t length;
	a65_object_header_t *header;
	a65_archive_payload_t *payload;

	A65_DEBUG_ENTRY_INFO("Data[%u]=%p", data.size(), &data);

	length = data.size();
	if(length < sizeof(a65_object_header_t)) {
		A65_THROW_EXCEPTION_INFO("Invalid archive length", "%u (min=%u)", length, sizeof(a65_object_header_t));
	}

	header = (a65_object_header_t *)&data[0];
	if(!header) {
		A65_THROW_EXCEPTION("Malformed archive header");
	}

	if(header->magic != A65_OBJECT_MAGIC) {
		A65_THROW_EXCEPTION_INFO("Archive header mismatch", "Magic=%u(%08x) (expecting=%u(%08x))", header->magic, header->magic,
			A65_OBJECT_MAGIC, A65_OBJECT_MAGIC);
#ifdef VERSION_CHECK
	} else if((header->metadata.major != A65_VERSION_MAJOR) || (header->metadata.minor != A65_VERSION_MINOR)) {
		A65_THROW_EXCEPTION_INFO("Archive header mismatch", "Version=%u.%u (expecting=%u.%u)", header->metadata.major, header->metadata.minor,
			A65_VERSION_MAJOR, A65_VERSION_MINOR);
#endif // VERSION_CHECK
	} else if(header->metadata.type != A65_ARCHIVE_TYPE) {
		A65_THROW_EXCEPTION_INFO("Archive header mismatch", "Type=%u(%04x) (expecting=%u(%04x))", header->metadata.type, header->metadata.type,
			A65_ARCHIVE_TYPE, A65_ARCHIVE_TYPE);
	}

	clear();

	if(length > sizeof(a65_object_header_t)) {
		uint32_t size;

		payload = (a65_archive_payload_t *)&data[sizeof(a65_object_header_t)];
		if(!payload) {
			A65_THROW_EXCEPTION("Malformed archive payload");
		}

		if(payload->metadata.size < sizeof(a65_archive_payload_t)) {
			A65_THROW_EXCEPTION_INFO("Archive payload length mismatch", "%u (min=%u)", payload->metadata.size,
				sizeof(a65_archive_payload_t));
		}

		size = payload->metadata.size;

		for(uint32_t entry = 0; entry < payload->metadata.count; ++entry) {

			a65_archive_object_t *object = (a65_archive_object_t *)&payload->object[entry];
			if(!object) {
				A65_THROW_EXCEPTION_INFO("Malformed archive payload object", "%u", entry);
			}

			if((object->offset > size) || ((object->offset + object->size) > size)) {
				A65_THROW_EXCEPTION_INFO("Archive payload object out-of-bounds", "%u (%u - %u))", entry, object->offset,
					object->offset + object->size);
			}
		}

		m_payload = (a65_archive_payload_t *) new uint8_t[size];
		if(!m_payload) {
			A65_THROW_EXCEPTION("Archive payload allocation failed");
		}

		std::memcpy(m_payload, payload, size);
		m_payload_size = size;
	}

	std::memcpy(&m_header, header, sizeof(m_header));

	A65_DEBUG_EXIT();
}

void
a65_archive::object(
	__in size_t position,
	__inout a65_object &object
	) const
{
	a65_archive_object_t *entry;

	A65_DEBUG_ENTRY_INFO("Position=%u", position);

	if(!contains_object(position)) {
		A65_THROW_EXCEPTION_INFO("Archive does not object", "%u", position);
	}

	entry = &m_payload->object[position];
	if(!entry) {
		A65_THROW_EXCEPTION_INFO("Malformed archive payload object", "%u", position);
	}

	std::vector<uint8_t> data = std::vector<uint8_t>(&((char *)m_payload)[entry->offset],
		&((char *)m_payload)[entry->offset] + entry->size);

	object.import(data);

	A65_DEBUG_EXIT();
}

void
a65_archive::read(
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

size_t
a65_archive::size(void) const
{
	size_t result;

	A65_DEBUG_ENTRY();

	result = (m_payload_size + sizeof(a65_object_header_t));

	A65_DEBUG_EXIT_INFO("Result=%u", result);
	return result;
}

std::string
a65_archive::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	if(m_payload) {

		for(uint32_t entry = 0; entry < m_payload->metadata.count; ++entry) {

			a65_archive_object_t *object = &m_payload->object[entry];
			if(!object) {
				A65_THROW_EXCEPTION_INFO("Malformed archive payload object", "%u", entry);
			}

			result << std::endl << "{" << A65_STRING_HEX(uint32_t, m_id) << "} [" << entry << "]"
				<< " {" << A65_STRING_HEX(uint32_t, object->offset) << ", " << A65_STRING_HEX(uint32_t, object->size) << "}";

			std::vector<uint8_t> data = std::vector<uint8_t>(&((char *)m_payload)[object->offset],
				&((char *)m_payload)[object->offset] + object->size);

			if(!data.empty()) {
				result << a65_object(data).to_string();
			}
		}
	}

	A65_DEBUG_EXIT();

	return result.str();
}

void
a65_archive::write(
	__in const std::string &path
	) const
{
	A65_DEBUG_ENTRY_INFO("Path[%u]=%s", path.size(), A65_STRING_CHECK(path));

	a65_utility::write_file(path, as_data());

	A65_DEBUG_EXIT();
}
