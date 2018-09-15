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

#ifndef A65_PARSER_H_
#define A65_PARSER_H_

#include "./a65_lexer.h"
#include "./a65_parser_type.h"
#include "./a65_tree.h"

class a65_parser :
		protected a65_lexer {

	public:

		a65_parser(
			__in_opt const std::string &input = std::string(),
			__in_opt bool is_path = true
			);

		a65_parser(
			__in const a65_parser &other
			);

		virtual ~a65_parser(void);

		a65_parser &operator=(
			__in const a65_parser &other
			);

		virtual void clear(void) override;

		virtual bool contains(
			__in uint32_t id
			) const override;

		virtual bool has_next(void) const override;

		virtual bool has_previous(void) const override;

		virtual void load(
			__in const std::string &input,
			__in_opt bool is_path = true
			) override;

		virtual void move_next(void) override;

		virtual void move_previous(void) override;

		virtual void reset(void) override;

		virtual std::string to_string(void) const override;

		a65_tree tree(
			__in_opt uint32_t id = A65_UUID_INVALID
			) const;

	protected:

		void add(
			__in const a65_token &token,
			__in_opt size_t position = A65_TREE_POSITION_UNDEFINED
			);

		void as_string(
			__in a65_tree &tree,
			__inout std::stringstream &stream,
			__inout size_t tabs
			) const;

		a65_tree evaluate(void);

		// TODO: add evaluation routines

		std::map<uint32_t, a65_tree>::iterator find(
			__in uint32_t id
			);

		std::vector<uint32_t> m_tree;

		std::map<uint32_t, a65_tree> m_tree_map;

		size_t m_tree_position;
};

#endif // A65_PARSER_H_
