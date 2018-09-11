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

#define A65_CHARACTER_FILL '.'

enum {
	A65_STREAM_CHARACTER_ALPHA = 0,
	A65_STREAM_CHARACTER_DIGIT,
	A65_STREAM_CHARACTER_END,
	A65_STREAM_CHARACTER_SPACE,
	A65_STREAM_CHARACTER_SYMBOL,
};

#define A65_STREAM_CHARACTER_MAX A65_STREAM_CHARACTER_SYMBOL

static const std::string A65_STREAM_CHARACTER_STR[] = {
	"Alpha", "Digit", "End", "Space", "Symbol",
	};

#define A65_STREAM_CHARACTER_STRING(_TYPE_) \
	(((_TYPE_) > A65_STREAM_CHARACTER_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_STREAM_CHARACTER_STR[_TYPE_]))

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

		char character(void) const;

		int character_type(void) const;

		virtual void clear(void);

		virtual bool has_next(void) const;

		virtual bool has_previous(void) const;

		bool is_binary(void) const;

		bool is_decimal(void) const;

		bool is_hexidecimal(void) const;

		bool is_octal(void) const;

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
			__in char value
			) const;

		virtual void move_next(void);

		virtual void move_previous(void);

		std::string path(void) const;

		virtual void reset(void);

		virtual std::string to_string(void) const;

	protected:

		std::string m_character;

		size_t m_character_position;

		size_t m_line;

		std::string m_path;
};

#endif // A65_STREAM_H_
