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

#include <sstream>
#include "../inc/a65_lexer.h"
#include "../inc/a65_utility.h"

#define A65_CHARACTER_COMMENT ';'
#define A65_CHARACTER_DIGIT_BINARY 'b'
#define A65_CHARACTER_DIGIT_HEXIDECIMAL 'x'
#define A65_CHARACTER_DIGIT_OCTAL 'c'
#define A65_CHARACTER_DIRECTIVE '.'
#define A65_CHARACTER_ESCAPE '\\'
#define A65_CHARACTER_LABEL ':'
#define A65_CHARACTER_LITERAL '\"'
#define A65_CHARACTER_LITERAL_CHARACTER '\''
#define A65_CHARACTER_NEWLINE '\n'
#define A65_CHARACTER_PRAGMA '@'
#define A65_CHARACTER_UNDERSCORE '_'
#define A65_CHARACTER_ZERO '0'

#define A65_SCALAR_BINARY_BASE 2
#define A65_SCALAR_BINARY_LENGTH_MAX 16
#define A65_SCALAR_DECIMAL_LENGTH_MAX 5
#define A65_SCALAR_HEXIDECIMAL_LENGTH_MAX 4
#define A65_SCALAR_OCTAL_BASE 8
#define A65_SCALAR_OCTAL_LENGTH_MAX 6

#define A65_TOKEN_SENTINEL_COUNT 2

a65_lexer::a65_lexer(
	__in_opt const std::string &input,
	__in_opt bool is_path
	) :
		a65_stream(input, is_path),
		m_token_position(0)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%s (%s)", input.size(), A65_STRING_CHECK(input), is_path ? "File" : "Raw");

	a65_lexer::clear();

	A65_DEBUG_EXIT();
}

a65_lexer::a65_lexer(
	__in const a65_lexer &other
	) :
		a65_stream(other),
		m_token(other.m_token),
		m_token_map(other.m_token_map),
		m_token_position(other.m_token_position)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_lexer::~a65_lexer(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_lexer &
a65_lexer::operator=(
	__in const a65_lexer &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_stream::operator=(other);
		m_token = other.m_token;
		m_token_map = other.m_token_map;
		m_token_position = other.m_token_position;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_lexer::add(
	__in const a65_token &token,
	__in_opt size_t position
	)
{
	a65_uuid_t id;

	A65_DEBUG_ENTRY_INFO("Token=%s, Position=%u", A65_STRING_CHECK(token.to_string()), position);

	if(position == A65_TOKEN_POSITION_UNDEFINED) {
		position = (m_token_position + 1);
	}

	if(position > m_token.size()) {
		A65_THROW_EXCEPTION_INFO("Token position out-of-range", "%u (max=%u)", position, m_token.size());
	}

	id = token.id();

	if(position < m_token.size()) {
		m_token.insert(m_token.begin() + position, id);
	} else {
		m_token.push_back(id);
	}

	if(m_token_map.find(id) != m_token_map.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate token", "%u(%x)", id, id);
	}

	m_token_map.insert(std::make_pair(id, token));

	if(m_token_position > position) {
		++m_token_position;
	}

	A65_DEBUG_EXIT();
}

void
a65_lexer::clear(void)
{
	A65_DEBUG_ENTRY();

	a65_stream::reset();
	m_token.clear();
	m_token_map.clear();
	m_token_position = 0;
	add(a65_token(A65_TOKEN_BEGIN), 0);
	add(a65_token(A65_TOKEN_END), 1);

	A65_DEBUG_EXIT();
}

bool
a65_lexer::contains(
	__in a65_uuid_t id
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	result = (m_token_map.find(id) != m_token_map.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

a65_token
a65_lexer::enumerate(void)
{
	int type;
	a65_token result;

	A65_DEBUG_ENTRY();

	result.set_metadata(path(), line());

	type = character_type();
	switch(type) {
		case A65_STREAM_CHARACTER_ALPHA:
			enumerate_alpha(result);
			break;
		case A65_STREAM_CHARACTER_DIGIT:
			enumerate_digit(result);
			break;
		case A65_STREAM_CHARACTER_SYMBOL:
			enumerate_symbol(result);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Unexpected character type", "%u(%s) (%s:%u)", type, A65_STREAM_CHARACTER_STRING(type),
				A65_STRING_CHECK(path()), line());
	}

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}

void
a65_lexer::enumerate_alpha(
	__inout a65_token &token
	)
{
	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_DIRECTIVE)) {
		enumerate_alpha_directive(token);
	} else if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL)) {
		enumerate_alpha_literal(token);
	} else if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL_CHARACTER)) {
		enumerate_alpha_literal_character(token);
	} else if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_PRAGMA)) {
		enumerate_alpha_pragma(token);
	} else {
		std::string literal;

		literal += character();

		if(a65_stream::has_next()) {
			a65_stream::move_next();

			while(a65_stream::match(A65_STREAM_CHARACTER_ALPHA)
					|| a65_stream::match(A65_STREAM_CHARACTER_DIGIT)
					|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_UNDERSCORE)) {
				literal += character();
				a65_stream::move_next();
			}

			A65_STRING_LOWER(literal);

			if(A65_IS_TOKEN_COMMAND(literal)) {
				token.set(A65_TOKEN_COMMAND, A65_TOKEN_COMMAND_ID(literal));
			} else if(A65_IS_TOKEN_CONSTANT(literal)) {
				token.set(A65_TOKEN_CONSTANT, A65_TOKEN_CONSTANT_ID(literal));
			} else if(A65_IS_TOKEN_MACRO(literal)) {
				token.set(A65_TOKEN_MACRO, A65_TOKEN_MACRO_ID(literal));
			} else if(A65_IS_TOKEN_REGISTER(literal)) {
				token.set(A65_TOKEN_REGISTER, A65_TOKEN_REGISTER_ID(literal));
			} else if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LABEL)) {
				a65_stream::move_next();
				token.set(A65_TOKEN_LABEL);
				token.set_literal(literal);
			} else {
				token.set(A65_TOKEN_IDENTIFIER);
				token.set_literal(literal);
			}
		}
	}

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

char
a65_lexer::enumerate_alpha_character(void)
{
	char result;

	A65_DEBUG_ENTRY();

	if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_ESCAPE)) {

		// TODO: enumerate escape character
		result = '\0';
		// ---

	} else {
		result = character();
	}

	A65_DEBUG_EXIT_INFO("Result=\'%c\'(%02x)", std::isprint(result) ? result : A65_CHARACTER_FILL, result);
	return result;
}

void
a65_lexer::enumerate_alpha_directive(
	__inout a65_token &token
	)
{
	std::string literal;

	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	literal += character();

	if(!a65_stream::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	a65_stream::move_next();

	if(!a65_stream::match(A65_STREAM_CHARACTER_ALPHA)
			&& !a65_stream::match(A65_STREAM_CHARACTER_DIGIT)
			&& !a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_UNDERSCORE)) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	while(a65_stream::match(A65_STREAM_CHARACTER_ALPHA)
			|| a65_stream::match(A65_STREAM_CHARACTER_DIGIT)
			|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_UNDERSCORE)) {
		literal += character();

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	A65_STRING_LOWER(literal);

	if(!A65_IS_TOKEN_DIRECTIVE(literal)) {
		A65_THROW_EXCEPTION_INFO("Unsupported directive", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	token.set(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ID(literal));

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

void
a65_lexer::enumerate_alpha_literal(
	__inout a65_token &token
	)
{
	std::string literal;

	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	if(!a65_stream::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated literal", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	a65_stream::move_next();

	while(!a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL)) {
		literal += enumerate_alpha_character();

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	if(!a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL)) {
		A65_THROW_EXCEPTION_INFO("Unterminated literal", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	if(a65_stream::has_next()) {
		a65_stream::move_next();
	}

	token.set(A65_TOKEN_LITERAL);
	token.set_literal(literal);

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

void
a65_lexer::enumerate_alpha_literal_character(
	__inout a65_token &token
	)
{
	std::string literal;

	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	if(!a65_stream::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated character", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	a65_stream::move_next();

	if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL_CHARACTER)) {
		A65_THROW_EXCEPTION_INFO("Empty character", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	literal += enumerate_alpha_character();

	if(!a65_stream::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated character", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	a65_stream::move_next();

	if(!a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL_CHARACTER)) {
		A65_THROW_EXCEPTION_INFO("Unterminated character", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	if(a65_stream::has_next()) {
		a65_stream::move_next();
	}

	token.set(A65_TOKEN_LITERAL);
	token.set_literal(literal);

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

void
a65_lexer::enumerate_alpha_pragma(
	__inout a65_token &token
	)
{
	std::string literal;

	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	literal += character();

	if(!a65_stream::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated pragma", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	a65_stream::move_next();

	if(!a65_stream::match(A65_STREAM_CHARACTER_ALPHA)
			&& !a65_stream::match(A65_STREAM_CHARACTER_DIGIT)
			&& !a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_UNDERSCORE)) {
		A65_THROW_EXCEPTION_INFO("Unterminated pragma", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	while(a65_stream::match(A65_STREAM_CHARACTER_ALPHA)
			|| a65_stream::match(A65_STREAM_CHARACTER_DIGIT)
			|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_UNDERSCORE)) {
		literal += character();

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	A65_STRING_LOWER(literal);

	if(!A65_IS_TOKEN_PRAGMA(literal)) {
		A65_THROW_EXCEPTION_INFO("Unsupported pragma", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
	}

	token.set(A65_TOKEN_PRAGMA, A65_TOKEN_PRAGMA_ID(literal));

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

void
a65_lexer::enumerate_digit(
	__inout a65_token &token
	)
{
	uint16_t scalar = 0;

	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	if(a65_stream::has_next() && a65_stream::match(A65_STREAM_CHARACTER_DIGIT, A65_CHARACTER_ZERO)) {
		a65_stream::move_next();

		if(a65_stream::match(A65_STREAM_CHARACTER_ALPHA)) {
			std::string value;

			value += a65_stream::character();
			A65_STRING_LOWER(value);

			switch(value.front()) {
				case A65_CHARACTER_DIGIT_BINARY:
					a65_stream::move_next();
					scalar = enumerate_digit_binary();
					break;
				case A65_CHARACTER_DIGIT_HEXIDECIMAL:
					a65_stream::move_next();
					scalar = enumerate_digit_hexidecimal();
					break;
				case A65_CHARACTER_DIGIT_OCTAL:
					a65_stream::move_next();
					scalar = enumerate_digit_octal();
					break;
				default:
					break;
			}
		} else if(a65_stream::match(A65_STREAM_CHARACTER_DIGIT)) {
			scalar = enumerate_digit_decimal();
		}
	} else {
		scalar = enumerate_digit_decimal();
	}

	token.set(A65_TOKEN_SCALAR);
	token.set_scalar(scalar);

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

uint16_t
a65_lexer::enumerate_digit_binary(void)
{
	uint32_t result;
	std::string value;

	A65_DEBUG_ENTRY();

	if(!a65_stream::is_binary()) {
		A65_THROW_EXCEPTION_INFO("Unterminated scalar", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	while(a65_stream::is_binary()) {
		value += a65_stream::character();

		if(value.size() > A65_SCALAR_BINARY_LENGTH_MAX) {
			A65_THROW_EXCEPTION_INFO("Scalar too large", "0%c%s (%s:%u)", A65_CHARACTER_DIGIT_BINARY, A65_STRING_CHECK(value),
				A65_STRING_CHECK(path()), line());
		}

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	result = std::stoul(value, nullptr, A65_SCALAR_BINARY_BASE);
	if(result > UINT16_MAX) {
		A65_THROW_EXCEPTION_INFO("Scalar overflow", "0%c%s (%s:%u)", A65_CHARACTER_DIGIT_BINARY, A65_STRING_CHECK(value),
			A65_STRING_CHECK(path()), line());
	}

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

uint16_t
a65_lexer::enumerate_digit_decimal(void)
{
	uint32_t result;
	std::string value;
	std::stringstream stream;

	A65_DEBUG_ENTRY();

	if(!a65_stream::is_decimal()) {
		A65_THROW_EXCEPTION_INFO("Unterminated scalar", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	while(a65_stream::is_decimal()) {
		value += a65_stream::character();

		if(value.size() > A65_SCALAR_DECIMAL_LENGTH_MAX) {
			A65_THROW_EXCEPTION_INFO("Scalar too large", "%s (%s:%u)", A65_STRING_CHECK(value), A65_STRING_CHECK(path()), line());
		}

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	stream << std::dec << value;
	stream >> result;

	if(result > UINT16_MAX) {
		A65_THROW_EXCEPTION_INFO("Scalar overflow", "%s (%s:%u)", A65_STRING_CHECK(value), A65_STRING_CHECK(path()), line());
	}

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

uint16_t
a65_lexer::enumerate_digit_hexidecimal(void)
{
	uint32_t result;
	std::string value;
	std::stringstream stream;

	A65_DEBUG_ENTRY();

	if(!a65_stream::is_hexidecimal()) {
		A65_THROW_EXCEPTION_INFO("Unterminated scalar", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	while(a65_stream::is_hexidecimal()) {
		value += a65_stream::character();

		if(value.size() > A65_SCALAR_HEXIDECIMAL_LENGTH_MAX) {
			A65_THROW_EXCEPTION_INFO("Scalar too large", "0%c%s (%s:%u)", A65_CHARACTER_DIGIT_HEXIDECIMAL, A65_STRING_CHECK(value),
				A65_STRING_CHECK(path()), line());
		}

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	stream << std::hex << value;
	stream >> result;

	if(result > UINT16_MAX) {
		A65_THROW_EXCEPTION_INFO("Scalar overflow", "0%c%s (%s:%u)", A65_CHARACTER_DIGIT_HEXIDECIMAL, A65_STRING_CHECK(value),
			A65_STRING_CHECK(path()), line());
	}

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

uint16_t
a65_lexer::enumerate_digit_octal(void)
{
	uint32_t result;
	std::string value;

	A65_DEBUG_ENTRY();

	if(!a65_stream::is_octal()) {
		A65_THROW_EXCEPTION_INFO("Unterminated scalar", "(%s:%u)", A65_STRING_CHECK(path()), line());
	}

	while(a65_stream::is_octal()) {
		value += a65_stream::character();

		if(value.size() > A65_SCALAR_OCTAL_LENGTH_MAX) {
			A65_THROW_EXCEPTION_INFO("Scalar too large", "0%c%s (%s:%u)", A65_CHARACTER_DIGIT_OCTAL, A65_STRING_CHECK(value),
				A65_STRING_CHECK(path()), line());
		}

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	result = std::stoul(value, nullptr, A65_SCALAR_OCTAL_BASE);

	if(result > UINT16_MAX) {
		A65_THROW_EXCEPTION_INFO("Scalar overflow", "0%c%s (%s:%u)", A65_CHARACTER_DIGIT_OCTAL, A65_STRING_CHECK(value),
			A65_STRING_CHECK(path()), line());
	}

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

void
a65_lexer::enumerate_symbol(
	__inout a65_token &token
	)
{
	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_DIRECTIVE)
			|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL)
			|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_LITERAL_CHARACTER)
			|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_PRAGMA)
			|| a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_UNDERSCORE)) {
		enumerate_alpha(token);
	} else {
		std::string literal;

		literal += character();

		if(a65_stream::has_next()) {
			a65_stream::move_next();

			if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL)) {

				literal += character();
				if(!A65_IS_TOKEN_SYMBOL(literal)) {
					literal.erase(literal.end() - 1);
				} else {
					a65_stream::move_next();
				}
			}
		}

		if(!A65_IS_TOKEN_SYMBOL(literal)) {
			A65_THROW_EXCEPTION_INFO("Unsupported symbol", "%s (%s:%u)", A65_STRING_CHECK(literal), A65_STRING_CHECK(path()), line());
		}

		token.set(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_ID(literal));
	}

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

std::map<a65_uuid_t, a65_token>::iterator
a65_lexer::find(
	__in a65_uuid_t id
	)
{
	std::map<a65_uuid_t, a65_token>::iterator result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	result = m_token_map.find(id);
	if(result == m_token_map.end()) {
		A65_THROW_EXCEPTION_INFO("Token not found", "%u(%x)", id, id);
	}

	A65_DEBUG_EXIT_INFO("Result={%u(%x), %s}", result->first, result->first, A65_STRING_CHECK(result->second.to_string()));
	return result;
}

bool
a65_lexer::has_next(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = !token().match(A65_TOKEN_END);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_lexer::has_previous(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_token_position > 0);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_lexer::load(
	__in const std::string &input,
	__in_opt bool is_path
	)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%s (%s)", input.size(), A65_STRING_CHECK(input), is_path ? "File" : "Raw");

	a65_stream::load(input, is_path);
	a65_lexer::clear();

	A65_DEBUG_EXIT();
}

void
a65_lexer::move_next(void)
{
	A65_DEBUG_ENTRY();

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION("No next token in lexer");
	}

	if(a65_stream::has_next()) {
		skip();

		if(m_token_position <= (m_token.size() - A65_TOKEN_SENTINEL_COUNT)) {
			add(enumerate());
			skip();
		}
	}

	++m_token_position;

	A65_DEBUG_EXIT();
}

void
a65_lexer::move_previous(void)
{
	A65_DEBUG_ENTRY();

	if(!a65_lexer::has_previous()) {
		A65_THROW_EXCEPTION("No previous token in lexer");
	}

	--m_token_position;

	A65_DEBUG_EXIT();
}

void
a65_lexer::remove(
	__in a65_uuid_t id
	)
{
	size_t position = 0;
	std::vector<a65_uuid_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	m_token_map.erase(find(id));

	for(entry = m_token.begin(); entry != m_token.end(); ++entry, ++position) {

		if(id == *entry) {
			break;
		}
	}

	if(entry != m_token.end()) {

		if(m_token_position >= position) {
			--m_token_position;
		}

		m_token.erase(entry);
	}

	A65_DEBUG_EXIT();
}

void
a65_lexer::reset(void)
{
	A65_DEBUG_ENTRY();

	m_token_position = 0;

	A65_DEBUG_EXIT();
}

void
a65_lexer::skip(void)
{
	A65_DEBUG_ENTRY();

	while(a65_stream::match(A65_STREAM_CHARACTER_SPACE)) {

		if(!a65_stream::has_next()) {
			break;
		}

		a65_stream::move_next();
	}

	if(a65_stream::match(A65_STREAM_CHARACTER_SYMBOL, A65_CHARACTER_COMMENT)) {

		while(!a65_stream::match(A65_STREAM_CHARACTER_SPACE, A65_CHARACTER_NEWLINE)) {

			if(!a65_stream::has_next()) {
				break;
			}

			a65_stream::move_next();
		}

		skip();
	}

	A65_DEBUG_EXIT();
}

std::string
a65_lexer::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result << "[" << m_token_position << "] " << token().to_string();

	A65_DEBUG_EXIT();
	return result.str();
}

a65_token
a65_lexer::token(
	__in_opt a65_uuid_t id
	) const
{
	a65_token result;
	std::map<a65_uuid_t, a65_token>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	if(id == A65_UUID_INVALID) {

		if(m_token_position >= m_token.size()) {
			A65_THROW_EXCEPTION_INFO("Token position out-of-range", "%u (max=%u)", m_token_position, m_token.size());
		}

		id = m_token.at(m_token_position);
	}

	entry = m_token_map.find(id);
	if(entry == m_token_map.end()) {
		A65_THROW_EXCEPTION_INFO("Token not found", "%u(%x)", id, id);
	}

	result = entry->second;

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}
