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

		virtual void clear(void) override;

		virtual void run(
			__in_opt const std::string &input = std::string(),
			__in_opt const std::string &output = std::string(),
			__in_opt int options = A65_ASSEMBLER_OPTION_DEFAULT
			);

	protected:

		void evaluate(void);

		// TODO: add additional evaluation routines

		void form_binary(void);

		void form_ihex(void);

		void form_listing(void);

		void move_child_tree(
			__in a65_tree &tree,
			__in size_t position
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

		std::string m_input;

		std::string m_output;

		std::map<uint16_t, a65_section> m_section;
};

#endif // A65_ASSEMBLER_H_
