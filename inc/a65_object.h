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

#ifndef A65_OBJECT_H_
#define A65_OBJECT_H_

#include "./a65_object_type.h"
#include "./a65_section.h"

class a65_object :
		public a65_id {

	public:

		explicit a65_object(
			__in_opt const std::map<uint16_t, a65_section> &section = std::map<uint16_t, a65_section>()
			);

		explicit a65_object(
			__in const std::vector<uint8_t> &data
			);

		explicit a65_object(
			__in const std::string &path
			);

		a65_object(
			__in const a65_object &other
			);

		virtual ~a65_object(void);

		a65_object &operator=(
			__in const a65_object &other
			);

		std::vector<uint8_t> as_data(
			__in_opt bool header = true
			) const;

		void clear(void);

		bool contains_section(
			__in const std::string &name
			) const;

		bool contains_section(
			__in size_t position
			) const;

		size_t count(void) const;

		bool empty(void) const;

		void import(
			__in const std::map<uint16_t, a65_section> &section
			);

		void import(
			__in const std::vector<uint8_t> &data
			);

		void read(
			__in const std::string &path
			);

		uint16_t section(
			__in const std::string &name,
			__inout std::vector<uint8_t> &data
			) const;

		uint16_t section(
			__in size_t position,
			__inout std::vector<uint8_t> &data
			) const;

		size_t size(
			__in_opt bool header = true
			) const;

		virtual std::string to_string(void) const;

		void write(
			__in const std::string &path
			) const;

	protected:

		void copy(
			__in const a65_object &other
			);

		std::map<std::string, uint32_t>::const_iterator find(
			__in const std::string &name
			) const;

		a65_object_header_t m_header;

		a65_object_payload_t *m_payload;

		size_t m_payload_size;

		std::map<std::string, uint32_t> m_section;
};

#endif // A65_OBJECT_H_
