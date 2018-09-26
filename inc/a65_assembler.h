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
		protected a65_parser {

	public:

		a65_assembler(
			__in const std::string &output,
			__in_opt const std::string &input = std::string(),
			__in_opt bool is_path = true
			);

		a65_assembler(
			__in const a65_assembler &other
			);

		virtual ~a65_assembler(void);

		a65_assembler &operator=(
			__in const a65_assembler &other
			);

		virtual void clear(void) override;

		virtual void evaluate(
			__in const std::string &output,
			__in const std::string &input,
			__in_opt bool is_path = true
			);

		virtual std::string to_string(void) const override;

	protected:

		std::string as_source(
			__in a65_tree &tree
			) const;

		void evaluate(void);

		// TODO: add additional evaluation routines

		void listing(void);

		void preprocess(void);

		std::map<uint16_t, a65_section> m_section;
};

#endif // A65_ASSEMBLER_H_
