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

#ifndef A65_UUID_H_
#define A65_UUID_H_

#include <map>
#include <set>
#include "./a65_define.h"

typedef uint32_t a65_uuid_t;

#define A65_UUID_INVALID (a65_uuid_t)0

class a65_uuid {

	public:

		~a65_uuid(void);

		bool contains(
			__in a65_uuid_t id
			) const;

		void decrement(
			__in a65_uuid_t id
			);

		a65_uuid_t generate(void);

		void increment(
			__in a65_uuid_t id
			);

		static a65_uuid &instance(void);

	protected:

		a65_uuid(void);

		a65_uuid(
			__in const a65_uuid &other
			) = delete;

		a65_uuid &operator=(
			__in const a65_uuid &other
			) = delete;

		std::map<a65_uuid_t, size_t>::iterator find(
			__in a65_uuid_t id
			);

		a65_uuid_t m_next;

		std::map<a65_uuid_t, size_t> m_reference;

		std::set<a65_uuid_t> m_surplus;
};

#endif // A65_UUID_H_
