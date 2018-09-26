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

#ifndef A65_STREAM_TYPE_H_
#define A65_STREAM_TYPE_H_

#include "./a65_define.h"

#define A65_CHARACTER_END '\0'
#define A65_CHARACTER_FILL '.'
#define A65_CHARACTER_NEGATION '-'
#define A65_CHARACTER_NEWLINE '\n'
#define A65_CHARACTER_SCALAR_BINARY_HIGH '1'
#define A65_CHARACTER_SCALAR_BINARY_LOW '0'
#define A65_CHARACTER_SCALAR_OCTAL_HIGH '7'
#define A65_CHARACTER_SCALAR_OCTAL_LOW '0'

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

#define A65_STREAM_LINE_START 1

#endif // A65_STREAM_TYPE_H_
