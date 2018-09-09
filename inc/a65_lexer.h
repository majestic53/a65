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

#include "./a65_token.h"

#define A65_TOKEN_POSITION_UNDEFINED (-1)

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

		void add(
			__in const a65_token &token,
			__in_opt size_t position = A65_TOKEN_POSITION_UNDEFINED
			);

		virtual void clear(void) override;

		virtual bool contains(
			__in a65_uuid_t id
			) const;

		virtual bool has_next(void) const override;

		virtual bool has_previous(void) const override;

		virtual void load(
			__in const std::string &input,
			__in_opt bool is_path = true
			) override;

		virtual void move_next(void) override;

		virtual void move_previous(void) override;

		virtual void remove(
			__in a65_uuid_t id
			);

		virtual void reset(void) override;

		virtual std::string to_string(void) const override;

		a65_token token(
			__in_opt a65_uuid_t id = A65_UUID_INVALID
			) const;

	protected:

		a65_token enumerate(void);

		void enumerate_alpha(
			__inout a65_token &token
			);

		void enumerate_digit(
			__inout a65_token &token
			);

		void enumerate_symbol(
			__inout a65_token &token
			);

		std::map<a65_uuid_t, a65_token>::iterator find(
			__in a65_uuid_t id
			);

		void skip(void);

		std::vector<a65_uuid_t> m_token;

		std::map<a65_uuid_t, a65_token> m_token_map;

		size_t m_token_position;
};

#endif // A65_LEXER_H_
