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

#ifndef A65_STREAM_H_
#define A65_STREAM_H_

#include <vector>
#include "./a65_define.h"

enum {
	A65_STREAM_CHARACTER_ALPHA = 0,
	A65_STREAM_CHARACTER_DIGIT,
	A65_STREAM_CHARACTER_END,
	A65_STREAM_CHARACTER_SPACE,
	A65_STREAM_CHARACTER_SYMBOL,
};

typedef uint8_t a65_char_t;
typedef std::vector<a65_char_t> a65_literal_t;

class a65_stream {

	public:

		a65_stream(
			__in_opt const std::string &input = std::string(),
			__in_opt bool is_path = true
			);

		a65_stream(
			__in const a65_stream &other
			);

		virtual ~a65_stream(void);

		a65_stream &operator=(
			__in const a65_stream &other
			);

		a65_char_t character(void) const;

		virtual void clear(void);

		virtual bool has_next(void) const;

		virtual bool has_previous(void) const;

		bool is_decimal(void) const;

		bool is_hexidecimal(void) const;

		size_t line(void) const;

		virtual void load(
			__in const std::string &input,
			__in_opt bool is_path = true
			);

		bool match(
			__in int type
			) const;

		bool match(
			__in int type,
			__in a65_char_t value
			) const;

		virtual void move_next(void);

		virtual void move_previous(void);

		std::string path(void) const;

		virtual void reset(void);

		virtual std::string to_string(void) const;

	protected:

		int type(void) const;

		a65_literal_t m_character;

		size_t m_character_position;

		size_t m_line;

		std::string m_path;
};

#endif // A65_STREAM_H_
