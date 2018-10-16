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

#ifndef A65_ARCHIVE_H_
#define A65_ARCHIVE_H_

#include "./a65_archive_type.h"
#include "./a65_object.h"

class a65_archive :
		public a65_id {

	public:

		explicit a65_archive(
			__in_opt const std::vector<a65_object> &object = std::vector<a65_object>()
			);

		explicit a65_archive(
			__in const std::vector<uint8_t> &data
			);

		explicit a65_archive(
			__in const std::string &path
			);

		a65_archive(
			__in const a65_archive &other
			);

		virtual ~a65_archive(void);

		a65_archive &operator=(
			__in const a65_archive &other
			);

		std::vector<uint8_t> as_data(
			__in_opt bool header = true
			) const;

		void clear(void);

		bool contains_object(
			__in size_t position
			) const;

		size_t count(void) const;

		bool empty(void) const;

		void import(
			__in const std::vector<a65_object> &object
			);

		void import(
			__in const std::vector<uint8_t> &data
			);

		void object(
			__in size_t position,
			__inout a65_object &object
			) const;

		void read(
			__in const std::string &path
			);

		size_t size(
			__in_opt bool header = true
			) const;

		virtual std::string to_string(void) const;

		void write(
			__in const std::string &path
			) const;

	protected:

		void copy(
			__in const a65_archive &other
			);

		a65_object_header_t m_header;

		a65_archive_payload_t *m_payload;

		size_t m_payload_size;
};

#endif // A65_ARCHIVE_H_
