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

#ifndef A65_LEXER_TYPE_H_
#define A65_LEXER_TYPE_H_

#include "./a65_stream_type.h"

#define A65_CHARACTER_COMMENT ';'
#define A65_CHARACTER_DIGIT_BINARY 'b'
#define A65_CHARACTER_DIGIT_HEXIDECIMAL 'x'
#define A65_CHARACTER_DIGIT_OCTAL 'c'
#define A65_CHARACTER_DIRECTIVE '.'
#define A65_CHARACTER_ESCAPE '\\'
#define A65_CHARACTER_ESCAPE_DECIMAL_LENGTH 3
#define A65_CHARACTER_ESCAPE_HEXIDECIMAL_LENGTH 2
#define A65_CHARACTER_LABEL ':'
#define A65_CHARACTER_LITERAL '\"'
#define A65_CHARACTER_LITERAL_CHARACTER '\''
#define A65_CHARACTER_PRAGMA '@'
#define A65_CHARACTER_TAB '\t'
#define A65_CHARACTER_UNDERSCORE '_'
#define A65_CHARACTER_ZERO '0'

#define A65_SCALAR_BINARY_BASE 2
#define A65_SCALAR_BINARY_LENGTH_MAX 16
#define A65_SCALAR_DECIMAL_LENGTH_MAX 5
#define A65_SCALAR_HEXIDECIMAL_LENGTH_MAX 4
#define A65_SCALAR_OCTAL_BASE 8
#define A65_SCALAR_OCTAL_LENGTH_MAX 6

#define A65_TOKEN_POSITION_UNDEFINED (-1)

#define A65_TOKEN_SENTINEL_COUNT 2

#endif // A65_LEXER_TYPE_H_
