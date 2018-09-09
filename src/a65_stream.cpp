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

#include <cctype>
#include <sstream>
#include "../inc/a65_stream.h"
#include "../inc/a65_utility.h"

#define A65_CHARACTER_END '\0'
#define A65_CHARACTER_FILL '.'
#define A65_CHARACTER_NEWLINE '\n'

#define A65_STREAM_LINE_START 1

a65_stream::a65_stream(
	__in_opt const std::string &input,
	__in_opt bool is_path
	) :
		m_character_position(0),
		m_line(A65_STREAM_LINE_START)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%s (%s)", input.size(), A65_STRING_CHECK(input), is_path ? "Path" : "Raw");

	a65_stream::load(input, is_path);

	A65_DEBUG_EXIT();
}

a65_stream::a65_stream(
	__in const a65_stream &other
	) :
		m_character(other.m_character),
		m_character_position(other.m_character_position),
		m_line(other.m_line),
		m_path(other.m_path)

{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_stream::~a65_stream(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_stream &
a65_stream::operator=(
	__in const a65_stream &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		m_character = other.m_character;
		m_character_position = other.m_character_position;
		m_line = other.m_line;
		m_path = other.m_path;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

a65_char_t
a65_stream::character(void) const
{
	a65_char_t result;

	A65_DEBUG_ENTRY();

	if(m_character_position >= m_character.size()) {
		A65_THROW_EXCEPTION_INFO("Character position out-of-range", "%u (max=%u)", m_character_position, m_character.size());
	}

	result = m_character.at(m_character_position);

	A65_DEBUG_EXIT_INFO("Result=\'%c\'(%02x)", std::isprint(result) ? (char)result : A65_CHARACTER_FILL, result);
	return result;
}

int
a65_stream::character_type(void) const
{
	int result;
	a65_char_t value;

	A65_DEBUG_ENTRY();

	value = character();
	if(value == A65_CHARACTER_END) {
		result = A65_STREAM_CHARACTER_END;
	} else if(std::isalpha(value)) {
		result = A65_STREAM_CHARACTER_ALPHA;
	} else if(std::isdigit(value)) {
		result = A65_STREAM_CHARACTER_DIGIT;
	} else if(std::isspace(value)) {
		result = A65_STREAM_CHARACTER_SPACE;
	} else {
		result = A65_STREAM_CHARACTER_SYMBOL;
	}

	A65_DEBUG_EXIT_INFO("Result=%i(%s)", result, A65_STREAM_CHARACTER_STRING(result));
	return result;
}

void
a65_stream::clear(void)
{
	A65_DEBUG_ENTRY();

	m_character.clear();
	m_character.push_back(A65_CHARACTER_END);
	m_character_position = 0;
	m_line = A65_STREAM_LINE_START;
	m_path.clear();

	A65_DEBUG_EXIT();
}

bool
a65_stream::has_next(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = !a65_stream::match(A65_STREAM_CHARACTER_END);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_stream::has_previous(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_character_position > 0);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_stream::is_decimal(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (std::isdigit(character()) > 0);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_stream::is_hexidecimal(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (std::isxdigit(character()) > 0);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

size_t
a65_stream::line(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u", m_line);
	return m_line;
}

void
a65_stream::load(
	__in const std::string &input,
	__in_opt bool is_path
	)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%s (%s)", input.size(), A65_STRING_CHECK(input), is_path ? "Path" : "Raw");

	a65_stream::clear();

	if(is_path) {
		a65_utility::read_file(input, m_character);
		m_path = input;
	} else {
		m_character = a65_literal_t(input.begin(), input.end());
		m_path = A65_STRING_UNKNOWN;
	}

	m_character.push_back(A65_CHARACTER_END);

	A65_DEBUG_EXIT();
}

bool
a65_stream::match(
	__in int type
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Type=%i(%s)", type, A65_STREAM_CHARACTER_STRING(type));

	result = (type == character_type());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_stream::match(
	__in int type,
	__in a65_char_t value
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Type=%i(%s), Value=\'%c\'(%02x)", type, A65_STREAM_CHARACTER_STRING(type),
		std::isprint(value) ? (char)value : A65_CHARACTER_FILL, value);

	result = ((type == character_type()) && (value == character()));

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_stream::move_next(void)
{
	A65_DEBUG_ENTRY();

	if(!a65_stream::has_next()) {
		A65_THROW_EXCEPTION("No next character in stream");
	}

	if(match(A65_STREAM_CHARACTER_SPACE, A65_CHARACTER_NEWLINE)) {
		++m_line;
	}

	++m_character_position;

	A65_DEBUG_EXIT();
}

void
a65_stream::move_previous(void)
{
	A65_DEBUG_ENTRY();

	if(!a65_stream::has_previous()) {
		A65_THROW_EXCEPTION("No previous character in stream");
	}

	--m_character_position;

	if(match(A65_STREAM_CHARACTER_SPACE, A65_CHARACTER_NEWLINE)) {
		--m_line;
	}

	A65_DEBUG_EXIT();
}

std::string
a65_stream::path(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", m_path.size(), A65_STRING_CHECK(m_path));
	return m_path;
}

void
a65_stream::reset(void)
{
	A65_DEBUG_ENTRY();

	m_character_position = 0;
	m_line = A65_STREAM_LINE_START;

	A65_DEBUG_EXIT();
}

std::string
a65_stream::to_string(void) const
{
	int type;
	a65_char_t value;
	std::stringstream result;

	A65_DEBUG_ENTRY();

	value = character();
	type = character_type();

	result << "[" << m_character_position << "] {" << A65_STREAM_CHARACTER_STRING(type) << "}";

	if(type != A65_STREAM_CHARACTER_END) {
		result << " \'" << (std::isprint(value) ? (char)value : A65_CHARACTER_FILL)
			<< "\' (" << A65_STRING_HEX(a65_char_t, value) << ") (";

		if(!m_path.empty()) {
			result << m_path << ":";
		}

		result << m_line << ")";
	}

	A65_DEBUG_EXIT();
	return result.str();
}
