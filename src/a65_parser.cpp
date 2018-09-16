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

#include "../inc/a65_parser.h"
#include "../inc/a65_utility.h"

a65_parser::a65_parser(
	__in_opt const std::string &input,
	__in_opt bool is_path
	) :
		a65_lexer(input, is_path),
		m_tree_position(0)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%s (%s)", input.size(), A65_STRING_CHECK(input), is_path ? "Path" : "Raw");

	a65_parser::clear();

	A65_DEBUG_EXIT();
}

a65_parser::a65_parser(
	__in const a65_parser &other
	) :
		a65_lexer(other),
		m_tree(other.m_tree),
		m_tree_map(other.m_tree_map),
		m_tree_position(other.m_tree_position)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_parser::~a65_parser(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_parser &
a65_parser::operator=(
	__in const a65_parser &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_lexer::operator=(other);
		m_tree = other.m_tree;
		m_tree_map = other.m_tree_map;
		m_tree_position = other.m_tree_position;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_parser::add(
	__in const a65_token &token,
	__in_opt size_t position
	)
{
	int type;
	a65_tree tree;

	A65_DEBUG_ENTRY_INFO("Token=%s, Position=%u", A65_STRING_CHECK(token.to_string()), position);

	switch(token.type()) {
		case A65_TOKEN_BEGIN:
			type = A65_TREE_BEGIN;
			break;
		case A65_TOKEN_END:
			type = A65_TREE_END;
			break;
		default:
			type = A65_TREE_STATEMENT;
			break;
	}

	tree.set(type);

	switch(token.type()) {
		case A65_TOKEN_BEGIN:
			type = A65_NODE_BEGIN;
			break;
		case A65_TOKEN_COMMAND:
			type = A65_NODE_COMMAND;
			break;
		case A65_TOKEN_DIRECTIVE:
			type = A65_NODE_DIRECTIVE;
			break;
		case A65_TOKEN_END:
			type = A65_NODE_END;
			break;
		case A65_TOKEN_PRAGMA:
			type = A65_NODE_PRAGMA;
			break;
		default:
			type = A65_NODE_LEAF;
			break;
	}

	tree.add(type, token.id());
	add(tree, position);

	A65_DEBUG_EXIT();
}

void
a65_parser::add(
	__in const a65_tree &tree,
	__in_opt size_t position
	)
{
	uint32_t id;

	A65_DEBUG_ENTRY_INFO("Tree=%s, Position=%u", A65_STRING_CHECK(tree.to_string()), position);

	if(position == A65_TREE_POSITION_UNDEFINED) {
		position = (m_tree_position + 1);
	}

	if(position > m_tree.size()) {
		A65_THROW_EXCEPTION_INFO("Tree position out-of-range", "%u (max=%u)", position, m_tree.size());
	}

	id = tree.id();

	if(m_tree_map.find(id) != m_tree_map.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate tree", "%u(%x)", id, id);
	}

	m_tree_map.insert(std::make_pair(id, tree));

	if(position < m_tree.size()) {
		m_tree.insert(m_tree.begin() + position, id);
	} else {
		m_tree.push_back(id);
	}

	if(m_tree_position > position) {
		++m_tree_position;
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::as_string(
	__in a65_tree &tree,
	__inout std::stringstream &stream,
	__inout size_t tabs
	) const
{
	a65_node node;
	size_t tab = 0;

	A65_DEBUG_ENTRY_INFO("Tree=%s, Stream=%p, Tabs=%u", A65_STRING_CHECK(tree.to_string()), &stream, tabs);

	stream << std::endl;

	for(; tab < tabs; ++tab) {
		stream << "\t";
	}

	node = tree.node();
	stream << node.to_string();

	if(node.has_token()) {
		stream << " " << a65_lexer::token(node.token()).to_string();
	}

	if(tree.has_child_left()) {
		tree.move_child_left();
		as_string(tree, stream, tabs + 1);
		tree.move_parent();
	}

	if(tree.has_child_right()) {
		tree.move_child_right();
		as_string(tree, stream, tabs + 1);
		tree.move_parent();
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::clear(void)
{
	A65_DEBUG_ENTRY();

	a65_lexer::reset();
	m_tree.clear();
	m_tree_map.clear();
	m_tree_position = 0;
	add(token_begin(), 0);
	add(token_end(), 1);

	A65_DEBUG_EXIT();
}

bool
a65_parser::contains(
	__in uint32_t id
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	result = (m_tree_map.find(id) != m_tree_map.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

a65_tree
a65_parser::evaluate(void)
{
	a65_tree result;

	A65_DEBUG_ENTRY();

	// TODO: evaluate tree
	result.set(A65_TREE_STATEMENT);
	result.add(A65_NODE_COMMAND, token().id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}
	// ---

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}

std::map<uint32_t, a65_tree>::iterator
a65_parser::find(
	__in uint32_t id
	)
{
	std::map<uint32_t, a65_tree>::iterator result;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	result = m_tree_map.find(id);
	if(result == m_tree_map.end()) {
		A65_THROW_EXCEPTION_INFO("Tree not found", "%u(%x)", id, id);
	}

	A65_DEBUG_EXIT_INFO("Result={%u(%x), %s}", result->first, result->first, A65_STRING_CHECK(result->second.to_string()));
	return result;
}

bool
a65_parser::has_next(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = !tree().match(A65_TREE_END);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_parser::has_previous(void) const
{
	bool result;

	A65_DEBUG_ENTRY();

	result = (m_tree_position > 0);

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_parser::load(
	__in const std::string &input,
	__in_opt bool is_path
	)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%s (%s)", input.size(), A65_STRING_CHECK(input), is_path ? "Path" : "Raw");

	a65_lexer::load(input, is_path);
	a65_parser::clear();

	A65_DEBUG_EXIT();
}

void
a65_parser::move_next(void)
{
	A65_DEBUG_ENTRY();

	if(!a65_parser::has_next()) {
		A65_THROW_EXCEPTION("No next tree in parser");
	}

	if(a65_lexer::has_next() && (m_tree_position <= (m_tree.size() - A65_TREE_SENTINEL_COUNT))) {

		if(a65_lexer::token().match(A65_TOKEN_BEGIN)) {
			a65_lexer::move_next();
		}

		add(evaluate());
	}

	++m_tree_position;

	A65_DEBUG_EXIT();
}

void
a65_parser::move_previous(void)
{
	A65_DEBUG_ENTRY();

	if(!a65_parser::has_previous()) {
		A65_THROW_EXCEPTION("No previous tree in parser");
	}

	--m_tree_position;

	A65_DEBUG_EXIT();
}

void
a65_parser::reset(void)
{
	A65_DEBUG_ENTRY();

	m_tree_position = 0;

	A65_DEBUG_EXIT();
}

std::string
a65_parser::to_string(void) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY();

	a65_tree entry = tree();
	result << "[" << m_tree_position << "] " << entry.to_string();

	if(!entry.empty()) {
		as_string(entry, result, A65_TREE_STRING_OFFSET_START);
	}

	A65_DEBUG_EXIT();
	return result.str();
}

a65_tree
a65_parser::tree(
	__in_opt uint32_t id
	) const
{
	a65_tree result;
	std::map<uint32_t, a65_tree>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Id=%u(%x)", id, id);

	if(id == A65_UUID_INVALID) {

		if(m_tree_position >= m_tree.size()) {
			A65_THROW_EXCEPTION_INFO("Tree position out-of-range", "%u (max=%u)", m_tree_position, m_tree.size());
		}

		id = m_tree.at(m_tree_position);
	}

	entry = m_tree_map.find(id);
	if(entry == m_tree_map.end()) {
		A65_THROW_EXCEPTION_INFO("Tree not found", "%u(%x)", id, id);
	}

	result = entry->second;

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}

a65_tree
a65_parser::tree_begin(void) const
{
	a65_tree result;

	A65_DEBUG_ENTRY();

	result = tree(m_tree.front());

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}

a65_tree
a65_parser::tree_end(void) const
{
	a65_tree result;

	A65_DEBUG_ENTRY();

	result = tree(m_tree.back());

	A65_DEBUG_EXIT_INFO("Result=%s", A65_STRING_CHECK(result.to_string()));
	return result;
}
