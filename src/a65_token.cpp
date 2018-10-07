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

#include "../inc/a65_token.h"
#include "../inc/a65_utility.h"

a65_token::a65_token(
	__in_opt int type,
	__in_opt int subtype,
	__in_opt int mode
	) :
		m_line(0),
		m_mode(mode),
		m_scalar(0),
		m_subtype(subtype),
		m_type(type)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Subtype=%u(%x), Mode=%u(%s)", type, A65_TOKEN_STRING(type), subtype, subtype, mode,
		A65_TOKEN_COMMAND_MODE_STRING(mode));
	A65_DEBUG_EXIT();
}

a65_token::a65_token(
	__in const a65_token &other
	) :
		a65_id(other),
		m_line(other.m_line),
		m_literal(other.m_literal),
		m_mode(other.m_mode),
		m_path(other.m_path),
		m_scalar(other.m_scalar),
		m_subtype(other.m_subtype),
		m_type(other.m_type)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_token::~a65_token(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_token &
a65_token::operator=(
	__in const a65_token &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_id::operator=(other);
		m_line = other.m_line;
		m_literal = other.m_literal;
		m_mode = other.m_mode;
		m_path = other.m_path;
		m_scalar = other.m_scalar;
		m_subtype = other.m_subtype;
		m_type = other.m_type;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

size_t
a65_token::line(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u", m_line);
	return m_line;
}

std::string
a65_token::literal(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", m_literal.size(), A65_STRING_CHECK(m_literal));
	return m_literal;
}

std::string
a65_token::literal_formatted(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	for(std::string::const_iterator ch = m_literal.begin(); ch != m_literal.end(); ++ch) {

		if(A65_IS_CHARACTER_ESCAPE_VALUE(*ch)) {
			int type = A65_CHARACTER_ESCAPE_VALUE_ID(*ch);

			if(type == A65_CHARACTER_ESCAPE_HEXIDECIMAL) {
				result << A65_CHARACTER_ESCAPE_STRING(A65_CHARACTER_ESCAPE_HEXIDECIMAL)
					<< A65_STRING_HEX(uint8_t, *ch);
			} else {
				result << A65_CHARACTER_ESCAPE_STRING(type);
			}
		} else if(!std::isprint(*ch)) {
			result << A65_CHARACTER_ESCAPE_STRING(A65_CHARACTER_ESCAPE_HEXIDECIMAL)
				<< A65_STRING_HEX(uint8_t, *ch);
		} else {
			result << *ch;
		}
	}

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

bool
a65_token::match(
	__in int type,
	__in_opt int subtype,
	__in_opt int mode
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Subtype=%u(%x), Mode=%u(%s)", type, A65_TOKEN_STRING(type), subtype, subtype, mode,
		A65_TOKEN_COMMAND_MODE_STRING(mode));

	result = (type == m_type);
	if(result && (subtype != A65_TOKEN_SUBTYPE_UNDEFINED)) {

		result = (subtype == m_subtype);
		if(result && (mode != A65_TOKEN_MODE_UNDEFINED)) {
			result = (mode == m_mode);
		}
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

int
a65_token::mode(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_mode, m_mode);
	return m_mode;
}

std::string
a65_token::path(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", m_path.size(), A65_STRING_CHECK(m_path));
	return m_path;
}

uint16_t
a65_token::scalar(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", m_scalar, m_scalar);
	return m_scalar;
}

void
a65_token::set(
	__in int type,
	__in_opt int subtype,
	__in_opt int mode
	)
{
	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Subtype=%u(%x), Mode=%u(%s)", type, A65_TOKEN_STRING(type), subtype, subtype, mode,
		A65_TOKEN_COMMAND_MODE_STRING(mode));

	m_mode = mode;
	m_subtype = subtype;
	m_type = type;

	A65_DEBUG_EXIT();
}

void
a65_token::set_literal(
	__in const std::string &literal
	)
{
	A65_DEBUG_ENTRY_INFO("Literal[%u]=%s", literal.size(), A65_STRING_CHECK(literal));

	m_literal = literal;

	A65_DEBUG_EXIT();
}

void
a65_token::set_metadata(
	__in const std::string &path,
	__in size_t line
	)
{
	A65_DEBUG_ENTRY_INFO("Path[%u]=%s, Line=%u", path.size(), A65_STRING_CHECK(path), line);

	m_line = line;
	m_path = path;

	A65_DEBUG_EXIT();
}

void
a65_token::set_mode(
	__in int mode
	)
{
	A65_DEBUG_ENTRY_INFO("Mode=%u(%s)", mode, A65_TOKEN_COMMAND_MODE_STRING(mode));

	m_mode = mode;

	A65_DEBUG_EXIT();
}

void
a65_token::set_scalar(
	__in uint16_t scalar
	)
{
	A65_DEBUG_ENTRY_INFO("Scalar=%u(%04x)", scalar, scalar);

	m_scalar = scalar;

	A65_DEBUG_EXIT();
}

int
a65_token::subtype(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_subtype, m_subtype);
	return m_subtype;
}

std::string
a65_token::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	result << "{" << a65_id::to_string() << "} [" << A65_TOKEN_STRING(m_type) << "]";

	switch(m_type) {
		case A65_TOKEN_BEGIN:
		case A65_TOKEN_END:
			break;
		default:

			switch(m_type) {
				case A65_TOKEN_IDENTIFIER:
				case A65_TOKEN_LABEL:
					result << "[" << m_literal.size() << "]";

					if(!m_literal.empty()) {
						result << " \"" << A65_STRING_CHECK(m_literal) << "\"";
					}
					break;
				case A65_TOKEN_LITERAL:
					result << "[" << m_literal.size() << "] \"" << literal_formatted() << "\"";
					break;
				case A65_TOKEN_SCALAR:
					result << " " << m_scalar << "(" << A65_STRING_HEX(uint16_t, m_scalar) << ")";
					break;
				default:
					break;
			}

			if(m_subtype != A65_TOKEN_SUBTYPE_UNDEFINED) {

				switch(m_type) {
					case A65_TOKEN_COMMAND:
						result << " " << A65_TOKEN_COMMAND_STRING(m_subtype);

						if(m_subtype == A65_TOKEN_COMMAND_CMD) {
							result << "<" << m_scalar << "(" << A65_STRING_HEX(uint8_t, m_scalar) << ")>";
						}

						if(m_mode != A65_TOKEN_MODE_UNDEFINED) {
							result << " " << A65_TOKEN_COMMAND_MODE_STRING(m_mode);
						}
						break;
					case A65_TOKEN_CONSTANT:
						result << " " << A65_TOKEN_CONSTANT_STRING(m_subtype);
						break;
					case A65_TOKEN_DIRECTIVE:
						result << " " << A65_TOKEN_DIRECTIVE_STRING(m_subtype);
						break;
					case A65_TOKEN_MACRO:
						result << " " << A65_TOKEN_MACRO_STRING(m_subtype);
						break;
					case A65_TOKEN_PRAGMA:
						result << " " << A65_TOKEN_PRAGMA_STRING(m_subtype);
						break;
					case A65_TOKEN_REGISTER:
						result << " " << A65_TOKEN_REGISTER_STRING(m_subtype);
						break;
					case A65_TOKEN_SYMBOL:
						result << " " << A65_TOKEN_SYMBOL_STRING(m_subtype);
						break;
					default:
						break;
				}
			}

			result << " (";

			if(!m_path.empty()) {
				result << A65_STRING_CHECK(m_path) << ":";
			}

			result << m_line << ")";
			break;
	}

	A65_DEBUG_EXIT();
	return result.str();
}

int
a65_token::type(void) const
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result=%u(%x)", m_type, m_type);
	return m_type;
}
