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

#ifndef A65_SECTION_H_
#define A65_SECTION_H_

#include "./a65_section_type.h"
#include "./a65_tree.h"

class a65_section {

	public:

		explicit a65_section(
			__in_opt uint16_t origin = 0
			);

		a65_section(
			__in const a65_section &other
			);

		virtual ~a65_section(void);

		a65_section &operator=(
			__in const a65_section &other
			);

		void add(
			__in const std::vector<uint8_t> &data,
			__in uint32_t listing
			);

		void clear(void);

		size_t count(void);

		std::vector<uint8_t> data(
			__in size_t position
			) const;

		bool empty(void) const;

		uint32_t id(void) const;

		uint32_t listing(
			__in size_t position
			) const;

		uint16_t offset(
			__in size_t position
			) const;

		uint16_t origin(void) const;

		void set_origin(
			__in uint16_t origin
			);

		uint16_t size(void) const;

		virtual std::string to_string(void) const;

	protected:

		void decrement(void);

		std::tuple<std::vector<uint8_t>, uint16_t, uint32_t> find(
			__in size_t position
			) const;

		void generate(void);

		void increment(void);

		uint32_t m_id;

		std::vector<std::tuple<std::vector<uint8_t>, uint16_t, uint32_t>> m_listing;

		uint16_t m_offset;

		uint16_t m_origin;
};

#endif // A65_SECTION_H_
