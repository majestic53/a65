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

		a65_tree tree_begin(void) const;

		a65_tree tree_end(void) const;

	protected:

		void add(
			__in const a65_token &token,
			__in_opt size_t position = A65_TREE_POSITION_UNDEFINED
			);

		void add(
			__in const a65_tree &tree,
			__in_opt size_t position = A65_TREE_POSITION_UNDEFINED
			);

		void as_string(
			__in a65_tree &tree,
			__inout std::stringstream &stream,
			__inout size_t tabs
			) const;

		void enumerate(
			__inout a65_tree &tree
			);

		void enumerate_command(
			__inout a65_tree &tree
			);

		void enumerate_directive(
			__inout a65_tree &tree
			);

		void enumerate_directive_data_byte(
			__inout a65_tree &tree
			);

		void enumerate_directive_data_word(
			__inout a65_tree &tree
			);

		void enumerate_directive_define(
			__inout a65_tree &tree
			);

		void enumerate_directive_else(
			__inout a65_tree &tree
			);

		void enumerate_directive_elseif(
			__inout a65_tree &tree
			);

		void enumerate_directive_end(
			__inout a65_tree &tree
			);

		void enumerate_directive_if(
			__inout a65_tree &tree
			);

		void enumerate_directive_if_define(
			__inout a65_tree &tree
			);

		void enumerate_directive_origin(
			__inout a65_tree &tree
			);

		void enumerate_directive_reserve(
			__inout a65_tree &tree
			);

		void enumerate_directive_undefine(
			__inout a65_tree &tree
			);

		void enumerate_expression(
			__inout a65_tree &tree
			);

		void enumerate_expression_arithmetic_0(
			__inout a65_tree &tree
			);

		void enumerate_expression_arithmetic_1(
			__inout a65_tree &tree
			);

		void enumerate_expression_binary(
			__inout a65_tree &tree
			);

		void enumerate_expression_condition(
			__inout a65_tree &tree
			);

		void enumerate_expression_factor(
			__inout a65_tree &tree
			);

		void enumerate_expression_list(
			__inout a65_tree &tree
			);

		void enumerate_expression_logical(
			__inout a65_tree &tree
			);

		void enumerate_label(
			__inout a65_tree &tree
			);

		void enumerate_pragma(
			__inout a65_tree &tree
			);

		void enumerate_pragma_include_binary(
			__inout a65_tree &tree
			);

		void enumerate_pragma_include_source(
			__inout a65_tree &tree
			);

		void enumerate_statement_list(
			__inout a65_tree &tree
			);

		std::map<uint32_t, a65_tree>::iterator find(
			__in uint32_t id
			);

		bool is_condition(void) const;

		bool is_expression(void) const;

		bool is_statement(void) const;

		bool is_valid_command_mode(
			__in int command,
			__in int mode
			) const;

		std::vector<uint32_t> m_tree;

		std::map<uint32_t, a65_tree> m_tree_map;

		size_t m_tree_position;
};

#endif // A65_PARSER_H_
