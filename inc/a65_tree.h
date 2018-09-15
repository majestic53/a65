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

#ifndef A65_TREE_H_
#define A65_TREE_H_

#include "./a65_node.h"
#include "./a65_tree_type.h"

class a65_tree {

	public:

		explicit a65_tree(
			__in_opt int type = A65_TREE_BEGIN
			);

		a65_tree(
			__in const a65_tree &other
			);

		virtual ~a65_tree(void);

		a65_tree &operator=(
			__in const a65_tree &other
			);

		void add(
			__in int type,
			__in uint32_t token
			);

		void add_child_left(
			__in int type,
			__in uint32_t token
			);

		void add_child_right(
			__in int type,
			__in uint32_t token
			);

		bool has_child_left(void) const;

		bool has_child_right(void) const;

		bool has_parent(void) const;

		bool has_root(void) const;

		uint32_t id(void) const;

		bool match(
			__in int type
			) const;

		void move_child_left(void);

		void move_child_right(void);

		void move_parent(void);

		void move_root(void);

		a65_node &node(
			__in_opt uint32_t id = A65_UUID_INVALID
			);

		void remove(void);

		void remove_child_left(void);

		void remove_child_right(void);

		virtual std::string to_string(void) const;

		int type(void) const;

	protected:

		void decrement(void);

		std::map<uint32_t, a65_node>::iterator find(
			__in uint32_t id
			);

		void generate(void);

		void increment(void);

		uint32_t m_id;

		uint32_t m_node;

		std::map<uint32_t, a65_node> m_node_map;

		uint32_t m_node_root;

		int m_type;
};

#endif // A65_TREE_H_
