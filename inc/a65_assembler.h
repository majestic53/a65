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

#ifndef A65_ASSEMBLER_H_
#define A65_ASSEMBLER_H_

#include "./a65_assembler_type.h"
#include "./a65_archive.h"
#include "./a65_parser.h"
#include "./a65_section.h"

class a65_assembler :
		public a65_parser {

	public:

		explicit a65_assembler(void);

		a65_assembler(
			__in const a65_assembler &other
			);

		virtual ~a65_assembler(void);

		a65_assembler &operator=(
			__in const a65_assembler &other
			);

		std::string build_archive(
			__in const std::vector<std::string> &input,
			__in const std::string &output,
			__in const std::string &name
			);

		std::string build_object(
			__in const std::string &input,
			__in const std::string &output,
			__in_opt bool source = false
			);

		virtual void clear(void) override;

		std::string compile(
			__in const std::vector<std::string> &input,
			__in const std::string &output,
			__in const std::string &name
			);

	protected:

		void add_define(
			__in const a65_token &token,
			__in uint16_t value
			);

		void add_export(
			__in const a65_token &token
			);

		void add_label(
			__in const a65_token &token,
			__in uint16_t origin
			);

		void add_section(
			__in const std::string &name,
			__in uint16_t origin
			);

		bool contains_define(
			__in const std::string &name
			) const;

		bool contains_export(
			__in const std::string &name
			) const;

		bool contains_label(
			__in const std::string &name
			) const;

		bool contains_section(
			__in uint16_t origin
			) const;

		void evaluate(
			__in const std::string &name,
			__in const std::string &input
			);

		std::vector<uint8_t> evaluate(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::vector<uint8_t> evaluate_command(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::vector<uint8_t> evaluate_directive(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		uint16_t evaluate_expression(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::vector<uint8_t> evaluate_list(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::vector<uint8_t> evaluate_pragma(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::map<std::string, uint16_t>::iterator find_define(
			__in const std::string &name
			);

		std::map<std::string, uint16_t>::iterator find_label(
			__in const std::string &name
			);

		std::map<uint16_t, a65_section>::iterator find_section(
			__in uint16_t origin
			);

		std::vector<uint8_t> form_command(
			__in int type,
			__in int mode,
			__in_opt uint16_t immediate = 0
			);

		bool is_valid_command(
			__in int type,
			__in int mode
			) const;

		void move_child_tree(
			__in a65_tree &tree,
			__in size_t position
			);

		std::string output_archive(
			__in const std::string &name,
			__in const std::vector<std::string> &input
			);

		std::string output_binary(
			__in const std::string &name,
			__in const std::vector<std::string> &input
			);

		std::string output_object(
			__in const std::string &name,
			__in_opt bool source = true
			);

		std::string output_object_source(
			__in const std::string &name,
			__in const a65_object &object
			);

		std::string output_source(
			__in const std::string &name,
			__in const std::string &source
			);

		std::string preprocess(
			__in_opt const std::string &input = std::string()
			);

		std::string preprocess(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::string preprocess_command(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::string preprocess_condition(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::string preprocess_directive(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::string preprocess_expression(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		std::string preprocess_pragma(
			__in a65_parser &parser,
			__in a65_tree &tree
			);

		void remove_define(
			__in const a65_token &token
			);

		std::map<std::string, uint16_t> m_define;

		std::set<std::string> m_export;

		std::string m_input;

		std::map<std::string, uint16_t> m_label;

		std::string m_name;

		uint16_t m_origin;

		std::string m_output;

		std::map<uint16_t, a65_section> m_section;
};

#endif // A65_ASSEMBLER_H_
