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

#ifndef A65_NODE_H_
#define A65_NODE_H_

#include "./a65_node_type.h"
#include "./a65_uuid.h"

class a65_node {

	public:

		a65_node(
			__in_opt int type = A65_NODE_BEGIN,
			__in_opt uint32_t token = A65_UUID_INVALID,
			__in_opt uint32_t parent = A65_UUID_INVALID,
			__in_opt uint32_t child_left = A65_UUID_INVALID,
			__in_opt uint32_t child_right = A65_UUID_INVALID
			);

		a65_node(
			__in const a65_node &other
			);

		virtual ~a65_node(void);

		a65_node &operator=(
			__in const a65_node &other
			);

		uint32_t child_left(void) const;

		uint32_t child_right(void) const;

		bool has_child_left(void) const;

		bool has_child_right(void) const;

		bool has_parent(void) const;

		bool has_token(void) const;

		uint32_t id(void) const;

		bool is_leaf(void) const;

		bool is_root(void) const;

		bool match(
			__in int type
			) const;

		uint32_t parent(void) const;

		void set(
			__in int type
			);

		void set_child_left(
			__in uint32_t id
			);

		void set_child_right(
			__in uint32_t id
			);

		void set_parent(
			__in uint32_t id
			);

		void set_token(
			__in uint32_t id
			);

		uint32_t token(void) const;

		virtual std::string to_string(void) const;

		int type(void) const;

	protected:

		void decrement(void);

		void generate(void);

		void increment(void);

		uint32_t m_child_left;

		uint32_t m_child_right;

		uint32_t m_id;

		uint32_t m_parent;

		uint32_t m_token;

		int m_type;
};

#endif // A65_NODE_H_
