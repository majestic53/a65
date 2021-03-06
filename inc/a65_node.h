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

#include "./a65_id.h"
#include "./a65_node_type.h"

class a65_node :
		public a65_id {

	public:

		a65_node(
			__in_opt int type = A65_NODE_BEGIN,
			__in_opt uint32_t token = A65_UUID_INVALID,
			__in_opt uint32_t parent = A65_UUID_INVALID,
			__in_opt const std::vector<uint32_t> &child = std::vector<uint32_t>()
			);

		a65_node(
			__in const a65_node &other
			);

		virtual ~a65_node(void);

		a65_node &operator=(
			__in const a65_node &other
			);

		size_t add_child(
			__in uint32_t id,
			__in_opt size_t position = A65_NODE_POSITION_UNDEFINED
			);

		uint32_t child(
			__in size_t position
			) const;

		size_t child_count(void) const;

		bool has_child(
			__in size_t position
			) const;

		bool has_parent(void) const;

		bool has_token(void) const;

		bool is_leaf(void) const;

		bool is_root(void) const;

		bool match(
			__in int type
			) const;

		uint32_t parent(void) const;

		void remove_all_children(void);

		void remove_child(
			__in size_t position
			);

		void set(
			__in int type
			);

		void set_child(
			__in uint32_t id,
			__in size_t position
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

		std::vector<uint32_t> m_child;

		uint32_t m_parent;

		uint32_t m_token;

		int m_type;
};

#endif // A65_NODE_H_
