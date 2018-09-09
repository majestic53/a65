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
#define A65_CHARACTER_NEWLINE '\n'

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
	a65_token result;

	A65_DEBUG_ENTRY();

	// TODO: enumerate token
	a65_literal_t literal;

	literal.push_back(a65_stream::character());

	result.set(A65_TOKEN_IDENTIFIER);
	result.set_literal(literal);

	if(a65_stream::has_next()) {
		a65_stream::move_next();
	}
	// ---

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}

void
a65_lexer::enumerate_alpha(
	__inout a65_token &token
	)
{
	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	// TODO: enumerate alpha token

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

void
a65_lexer::enumerate_digit(
	__inout a65_token &token
	)
{
	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	// TODO: enumerate digit token

	A65_DEBUG_ENTRY_INFO("Result=%s", A65_STRING_CHECK(token.to_string()));
}

void
a65_lexer::enumerate_symbol(
	__inout a65_token &token
	)
{
	A65_DEBUG_ENTRY_INFO("Token=%s", A65_STRING_CHECK(token.to_string()));

	// TODO: enumerate symbol token

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
