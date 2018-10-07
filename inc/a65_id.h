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

#ifndef A65_ID_H_
#define A65_ID_H_

#include "./a65_id_type.h"
#include "./a65_uuid.h"

class a65_id {

	public:

		a65_id(void);

		a65_id(
			__in const a65_id &other
			);

		virtual ~a65_id(void);

		a65_id &operator=(
			__in const a65_id &other
			);

		uint32_t id(void) const;

		std::string to_string(void) const;

	protected:

		void decrement(void);

		void generate(void);

		void increment(void);

		uint32_t m_id;
};

#endif // A65_ID_H_
