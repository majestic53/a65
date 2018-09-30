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

#ifndef A65_LEXER_H_
#define A65_LEXER_H_

#include "./a65_lexer_type.h"
#include "./a65_stream.h"
#include "./a65_token.h"

class a65_lexer :
		public a65_stream {

	public:

		a65_lexer(
			__in_opt const std::string &input = std::string(),
			__in_opt bool is_path = true
			);

		a65_lexer(
			__in const a65_lexer &other
			);

		virtual ~a65_lexer(void);

		a65_lexer &operator=(
			__in const a65_lexer &other
			);

		virtual void clear(void) override;

		virtual bool contains(
			__in uint32_t id
			) const;

		virtual bool has_next(void) const override;

		virtual bool has_previous(void) const override;

		virtual void load(
			__in const std::string &input,
			__in_opt bool is_path = true
			) override;

		virtual void move_next(void) override;

		virtual void move_previous(void) override;

		virtual void reset(void) override;

		void set_mode(
			__in uint32_t id,
			__in int mode
			);

		void set_scalar(
			__in uint32_t id,
			__in uint16_t scalar
			);

		virtual std::string to_string(void) const override;

		a65_token token(
			__in_opt uint32_t id = A65_UUID_INVALID
			) const;

		a65_token token_begin(void) const;

		a65_token token_end(void) const;

	protected:

		void add(
			__in const a65_token &token,
			__in_opt size_t position = A65_TOKEN_POSITION_UNDEFINED
			);

		a65_token enumerate(void);

		void enumerate_alpha(
			__inout a65_token &token
			);

		char enumerate_alpha_character(void);

		void enumerate_alpha_directive(
			__inout a65_token &token
			);

		void enumerate_alpha_literal(
			__inout a65_token &token
			);

		void enumerate_alpha_literal_character(
			__inout a65_token &token
			);

		void enumerate_alpha_pragma(
			__inout a65_token &token
			);

		void enumerate_digit(
			__inout a65_token &token,
			__in_opt bool negative = false
			);

		uint16_t enumerate_digit_binary(void);

		uint16_t enumerate_digit_decimal(void);

		uint16_t enumerate_digit_hexidecimal(void);

		uint16_t enumerate_digit_octal(void);

		void enumerate_symbol(
			__inout a65_token &token
			);

		std::map<uint32_t, a65_token>::iterator find(
			__in uint32_t id
			);

		void skip(void);

		std::vector<uint32_t> m_token;

		std::map<uint32_t, a65_token> m_token_map;

		size_t m_token_position;
};

#endif // A65_LEXER_H_
