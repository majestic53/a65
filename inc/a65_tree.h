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

class a65_tree :
		public a65_id {

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

		size_t add_child(
			__in int type,
			__in_opt uint32_t token = A65_UUID_INVALID,
			__in_opt size_t position = A65_NODE_POSITION_UNDEFINED
			);

		void add_root(
			__in int type,
			__in_opt uint32_t token = A65_UUID_INVALID
			);

		bool empty(void) const;

		bool has_child(
			__in size_t position
			) const;

		bool has_parent(void) const;

		bool has_root(void) const;

		bool match(
			__in int type
			) const;

		void move_child(
			__in size_t position
			);

		static void move_child(
			__in a65_tree &tree,
			__in size_t position
			);

		void move_parent(void);

		static void move_parent(
			__in a65_tree &tree
			);

		void move_root(void);

		a65_node &node(
			__in_opt uint32_t id = A65_UUID_INVALID
			);

		void remove_child(
			__in size_t position
			);

		void remove_root(void);

		virtual std::string to_string(void) const;

		void set(
			__in int type
			);

		int type(void) const;

	protected:

		std::map<uint32_t, a65_node>::iterator find(
			__in uint32_t id
			);

		void remove(void);

		uint32_t m_node;

		std::map<uint32_t, a65_node> m_node_map;

		uint32_t m_node_root;

		int m_type;
};

#endif // A65_TREE_H_
