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

#ifndef A65_TOKEN_H_
#define A65_TOKEN_H_

#include "./a65_stream.h"
#include "./a65_uuid.h"

#define A65_TOKEN_SUBTYPE_UNDEFINED (-1)

enum {
	A65_TOKEN_BEGIN = 0,
	A65_TOKEN_COMMAND,
	A65_TOKEN_CONSTANT,
	A65_TOKEN_DIRECTIVE,
	A65_TOKEN_END,
	A65_TOKEN_IDENTIFIER,
	A65_TOKEN_LABEL,
	A65_TOKEN_LITERAL,
	A65_TOKEN_MACRO,
	A65_TOKEN_PRAGMA,
	A65_TOKEN_REGISTER,
	A65_TOKEN_SCALAR,
	A65_TOKEN_SYMBOL,
};

class a65_token {

	public:

		a65_token(
			__in_opt int type = A65_TOKEN_BEGIN,
			__in_opt int subtype = A65_TOKEN_SUBTYPE_UNDEFINED
			);

		a65_token(
			__in const a65_token &other
			);

		virtual ~a65_token(void);

		a65_token &operator=(
			__in const a65_token &other
			);

		a65_uuid_t id(void) const;

		size_t line(void) const;

		a65_literal_t literal(void) const;

		bool match(
			__in int type,
			__in_opt int subtype = A65_TOKEN_SUBTYPE_UNDEFINED
			) const;

		std::string path(void) const;

		uint16_t scalar(void) const;

		void set(
			__in int type,
			__in_opt int subtype = A65_TOKEN_SUBTYPE_UNDEFINED
			);

		void set_literal(
			__in const a65_literal_t &literal
			);

		void set_metadata(
			__in const std::string &path,
			__in size_t line
			);

		void set_scalar(
			__in uint16_t scalar
			);

		int subtype(void) const;

		virtual std::string to_string(void) const;

		int type(void) const;

	protected:

		void decrement(void);

		void generate(void);

		void increment(void);

		a65_uuid_t m_id;

		size_t m_line;

		std::string m_literal;

		std::string m_path;

		uint16_t m_scalar;

		int m_subtype;

		int m_type;
};

#endif // A65_TOKEN_H_
