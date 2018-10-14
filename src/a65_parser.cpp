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
	A65_DEBUG_ENTRY_INFO("Input[%u]=%p (%s)", input.size(), &input, is_path ? "Path" : "Raw");

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
			type = A65_NODE_CONSTANT;
			break;
	}

	tree.add_root(type, token.id());
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

	A65_DEBUG_ENTRY_INFO("Tree=%p, Position=%u", &tree, position);

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
a65_parser::add_child(
	__inout a65_tree &tree,
	__in int type,
	__in_opt uint32_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p, Type=%u(%s), Id=%u(%x)", &tree, type, A65_NODE_STRING(type), id, id);

	if(tree.has_root()) {
		tree.add_child(type, id);
	} else {
		tree.add_root(type, id);
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::add_child_subtree(
	__inout a65_tree &tree,
	__in int type,
	__in_opt uint32_t id
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p, Type=%u(%s), Id=%u(%x)", &tree, type, A65_NODE_STRING(type), id, id);

	if(tree.has_root()) {
		tree.move_child(tree.add_child(type, id));
	} else {
		tree.add_root(type, id);
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::add_child_subtree(
	__inout a65_tree &tree,
	__in a65_tree &subtree
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p, Subtree=%p", &tree, &subtree);

	if(!subtree.empty()) {
		size_t child = 0;
		a65_node &node = subtree.node();

		if(tree.has_root()) {
			tree.move_child(tree.add_child(node.type(), node.token()));
		} else {
			tree.add_root(node.type(), node.token());
		}

		for(; child < node.child_count(); ++child) {
			subtree.move_child(child);
			add_child_subtree(tree, subtree);
			subtree.move_parent();
		}

		if(tree.has_parent()) {
			tree.move_parent();
		}
	}

	A65_DEBUG_EXIT();
}

std::string
a65_parser::as_string(
	__in a65_tree &tree,
	__inout size_t tabs
	) const
{
	a65_node node;
	std::stringstream result;
	size_t child = 0, tab = 0;

	A65_DEBUG_ENTRY_INFO("Tree=%p, Tabs=%u", &tree, tabs);

	result << std::endl;

	for(; tab < tabs; ++tab) {
		result << "\t";
	}

	node = tree.node();
	result << node.to_string();

	if(node.has_token()) {
		result << " " << a65_lexer::token(node.token()).to_string();
	}

	while(tree.has_child(child)) {
		tree.move_child(child++);
		result << as_string(tree, tabs + 1);
		tree.move_parent();
	}

	A65_DEBUG_EXIT();
	return result.str();
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

void
a65_parser::enumerate(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	if(!tree.has_root()) {
		tree.set(A65_TREE_STATEMENT);
	}

	entry = a65_lexer::token();

	switch(entry.type()) {
		case A65_TOKEN_COMMAND:
			enumerate_command(tree);
			break;
		case A65_TOKEN_DIRECTIVE:
			enumerate_directive(tree);
			break;
		case A65_TOKEN_LABEL:
			enumerate_label(tree);
			break;
		case A65_TOKEN_PRAGMA:
			enumerate_pragma(tree);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Unsupported token type", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_command(
	__inout a65_tree &tree
	)
{
	a65_token entry;
	int cmd, mode = A65_TOKEN_COMMAND_MODE_IMPLIED;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	cmd = entry.subtype();
	add_child_subtree(tree, A65_NODE_COMMAND, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	if(cmd == A65_TOKEN_COMMAND_CMD) {
		enumerate_command_custom(tree);
	}

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_CONSTANT)
			|| entry.match(A65_TOKEN_IDENTIFIER)
			|| entry.match(A65_TOKEN_LITERAL)
			|| entry.match(A65_TOKEN_MACRO)
			|| entry.match(A65_TOKEN_SCALAR)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_PARENTHESIS_OPEN)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_UNARY_NEGATION)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_UNARY_NOT)) {
		enumerate_expression(tree);

		if(!A65_IS_TOKEN_COMMAND_RELATIVE(cmd)) {
			mode = A65_TOKEN_COMMAND_MODE_ABSOLUTE;

			entry = a65_lexer::token();
			if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_SEPERATOR)) {

				if(!a65_lexer::has_next()) {
					A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				a65_lexer::move_next();

				entry = a65_lexer::token();
				if(entry.match(A65_TOKEN_REGISTER, A65_TOKEN_REGISTER_INDEX_X)) {
					mode = A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_X;
				} else if(entry.match(A65_TOKEN_REGISTER, A65_TOKEN_REGISTER_INDEX_Y)) {
					mode = A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_Y;
				} else {
					A65_THROW_EXCEPTION_INFO("Expecting index register", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				if(a65_lexer::has_next()) {
					a65_lexer::move_next();
				}
			}
		} else {
			mode = A65_TOKEN_COMMAND_MODE_RELATIVE;
		}
	} else if(entry.match(A65_TOKEN_REGISTER, A65_TOKEN_REGISTER_ACCUMULATOR)) {
		mode = A65_TOKEN_COMMAND_MODE_ACCUMULATOR;

		if(a65_lexer::has_next()) {
			a65_lexer::move_next();
		}
	} else if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_SQUARE_OPEN)) {

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression(tree);

		entry = a65_lexer::token();
		if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_SQUARE_CLOSE)) {
			mode = A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDIRECT;

			if(a65_lexer::has_next()) {
				a65_lexer::move_next();

				entry = a65_lexer::token();
				if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_SEPERATOR)) {
					mode = A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT_INDEX;
					a65_lexer::move_next();

					entry = a65_lexer::token();
					if(!entry.match(A65_TOKEN_REGISTER, A65_TOKEN_REGISTER_INDEX_Y)) {
						A65_THROW_EXCEPTION_INFO("Expecting index-y register", "%s", A65_STRING_CHECK(entry.to_string()));
					}

					a65_lexer::move_next();
				}
			}
		} else if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_SEPERATOR)) {
			mode = A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_INDIRECT;

			if(!a65_lexer::has_next()) {
				A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			a65_lexer::move_next();

			entry = a65_lexer::token();
			if(!entry.match(A65_TOKEN_REGISTER, A65_TOKEN_REGISTER_INDEX_X)) {
				A65_THROW_EXCEPTION_INFO("Expecting index-x register", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			if(!a65_lexer::has_next()) {
				A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			a65_lexer::move_next();

			entry = a65_lexer::token();
			if(!entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_SQUARE_CLOSE)) {
				A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			if(a65_lexer::has_next()) {
				a65_lexer::move_next();
			}
		}
	} else if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_IMMEDIATE)) {
		mode = A65_TOKEN_COMMAND_MODE_IMMEDIATE;

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression(tree);
	}

	set_mode(tree.node().token(), mode);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_command_custom(
	__inout a65_tree &tree
	)
{
	uint16_t scalar;
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_CURLY_OPEN)) {
		A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_SCALAR)) {
		A65_THROW_EXCEPTION_INFO("Expecting scalar", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	scalar = entry.scalar();
	if(scalar > UINT8_MAX) {
		A65_THROW_EXCEPTION_INFO("Scalar too large", "%s (max=%u)", A65_STRING_CHECK(entry.to_string()), UINT8_MAX);
	}

	set_scalar(tree.node().token(), scalar);

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_CURLY_CLOSE)) {
		A65_THROW_EXCEPTION_INFO("Unterminated command", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();

	switch(entry.subtype()) {
		case A65_TOKEN_DIRECTIVE_DATA_BYTE:
			enumerate_directive_data_byte(tree);
			break;
		case A65_TOKEN_DIRECTIVE_DATA_WORD:
			enumerate_directive_data_word(tree);
			break;
		case A65_TOKEN_DIRECTIVE_DEFINE:
			enumerate_directive_define(tree);
			break;
		case A65_TOKEN_DIRECTIVE_EXPORT:
			enumerate_directive_export(tree);
			break;
		case A65_TOKEN_DIRECTIVE_IF:
			enumerate_directive_if(tree);
			break;
		case A65_TOKEN_DIRECTIVE_IF_DEFINE:
		case A65_TOKEN_DIRECTIVE_IF_DEFINE_NOT:
			enumerate_directive_if_define(tree);
			break;
		case A65_TOKEN_DIRECTIVE_ORIGIN:
			enumerate_directive_origin(tree);
			break;
		case A65_TOKEN_DIRECTIVE_RESERVE:
			enumerate_directive_reserve(tree);
			break;
		case A65_TOKEN_DIRECTIVE_UNDEFINE:
			enumerate_directive_undefine(tree);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Unsupported directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_data_byte(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_expression_list(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_data_word(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_expression_list(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_define(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_IDENTIFIER)) {
		A65_THROW_EXCEPTION_INFO("Expecting identifier", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();

		entry = a65_lexer::token();
		if(entry.match(A65_TOKEN_CONSTANT)
				|| entry.match(A65_TOKEN_IDENTIFIER)
				|| entry.match(A65_TOKEN_LITERAL)
				|| entry.match(A65_TOKEN_MACRO)
				|| entry.match(A65_TOKEN_SCALAR)
				|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_PARENTHESIS_OPEN)
				|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_UNARY_NEGATION)
				|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_UNARY_NOT)) {
			enumerate_expression(tree);
		}
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_else(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_statement_list(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_elseif(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_expression_condition(tree);
	enumerate_statement_list(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_end(
	__inout a65_tree &tree
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_export(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_IDENTIFIER)) {
		A65_THROW_EXCEPTION_INFO("Expecting identifier", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());
	a65_tree::move_parent(tree);

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_if(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_expression_condition(tree);
	enumerate_statement_list(tree);

	entry = a65_lexer::token();
	while(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE_IF)) {
		enumerate_directive_elseif(tree);
		entry = a65_lexer::token();
	}

	if(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE)) {
		enumerate_directive_else(tree);
	}

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_END)) {
		A65_THROW_EXCEPTION_INFO("Expecting directive end", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	enumerate_directive_end(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_if_define(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_IDENTIFIER)) {
		A65_THROW_EXCEPTION_INFO("Expecting identifier", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Expecting directive end", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_statement_list(tree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE)) {
		enumerate_directive_else(tree);
	}

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_END)) {
		A65_THROW_EXCEPTION_INFO("Expecting directive end", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	enumerate_directive_end(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_origin(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_expression(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_reserve(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();
	enumerate_expression(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_directive_undefine(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_DIRECTIVE, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated directive", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = a65_lexer::token();
	if(!entry.match(A65_TOKEN_IDENTIFIER)) {
		A65_THROW_EXCEPTION_INFO("Expecting identifier", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression(
	__inout a65_tree &tree
	)
{
	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	add_child_subtree(tree, A65_NODE_EXPRESSION);
	enumerate_expression_binary(tree);
	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_arithmetic_0(
	__inout a65_tree &tree
	)
{
	a65_token entry;
	a65_tree subtree;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	enumerate_expression_arithmetic_1(subtree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_ARITHMETIC_ADDITION)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_ARITHMETIC_SUBTRACTION)) {
		add_child_subtree(tree, A65_NODE_OPERATOR, entry.id());
		add_child_subtree(tree, subtree);

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression_arithmetic_0(tree);
		a65_tree::move_parent(tree);
	} else {
		add_child_subtree(tree, subtree);
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_arithmetic_1(
	__inout a65_tree &tree
	)
{
	a65_token entry;
	a65_tree subtree;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	enumerate_expression_factor(subtree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_ARITHMETIC_DIVIDE)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_ARITHMETIC_MODULUS)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_ARITHMETIC_MULTIPLY)) {
		add_child_subtree(tree, A65_NODE_OPERATOR, entry.id());
		add_child_subtree(tree, subtree);

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression_arithmetic_1(tree);
		a65_tree::move_parent(tree);
	} else {
		add_child_subtree(tree, subtree);
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_binary(
	__inout a65_tree &tree
	)
{
	a65_token entry;
	a65_tree subtree;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	enumerate_expression_logical(subtree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BINARY_AND)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BINARY_OR)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BINARY_XOR)) {
		add_child_subtree(tree, A65_NODE_OPERATOR, entry.id());
		add_child_subtree(tree, subtree);

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression_binary(tree);
		a65_tree::move_parent(tree);
	} else {
		add_child_subtree(tree, subtree);
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_condition(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	add_child_subtree(tree, A65_NODE_CONDITION);
	enumerate_expression(tree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_AND)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_OR)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_EQUALS)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_GREATER_THAN)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_GREATER_THAN_EQUALS)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_LESS_THAN)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_LESS_THAN_EQUALS)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_OPERATOR_NOT_EQUALS)) {

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated condition", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		tree.node().set_token(entry.id());
		a65_lexer::move_next();
		enumerate_expression(tree);
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_factor(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_CONSTANT)
			|| entry.match(A65_TOKEN_SCALAR)) {
		add_child(tree, A65_NODE_CONSTANT, entry.id());

		if(a65_lexer::has_next()) {
			a65_lexer::move_next();
		}
	} else if(entry.match(A65_TOKEN_IDENTIFIER)
			|| entry.match(A65_TOKEN_LITERAL)) {
		add_child_subtree(tree, A65_NODE_CONSTANT, entry.id());

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();

		entry = a65_lexer::token();
		if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_CURLY_OPEN)) {

			if(!a65_lexer::has_next()) {
				A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			a65_lexer::move_next();
			enumerate_expression(tree);

			entry = a65_lexer::token();
			if(!entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_BRACE_CURLY_CLOSE)) {
				A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			if(a65_lexer::has_next()) {
				a65_lexer::move_next();
			}
		}

		a65_tree::move_parent(tree);
	} else if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_PARENTHESIS_OPEN)) {

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		add_child_subtree(tree, A65_NODE_EXPRESSION);
		enumerate_expression(tree);
		a65_tree::move_parent(tree);

		entry = a65_lexer::token();
		if(!entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_PARENTHESIS_CLOSE)) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		if(a65_lexer::has_next()) {
			a65_lexer::move_next();
		}
	} else if(entry.match(A65_TOKEN_MACRO)) {
		add_child_subtree(tree, A65_NODE_MACRO, entry.id());

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression_factor(tree);
		a65_tree::move_parent(tree);
	} else if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_UNARY_NEGATION)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_UNARY_NOT)) {
		add_child_subtree(tree, A65_NODE_UNARY, entry.id());

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression_factor(tree);
		a65_tree::move_parent(tree);
	} else {
		A65_THROW_EXCEPTION_INFO("Expecting expression", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_list(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	add_child_subtree(tree, A65_NODE_LIST);
	enumerate_expression(tree);

	entry = a65_lexer::token();
	while(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_SEPERATOR)) {

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated list", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression(tree);
		entry = a65_lexer::token();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_expression_logical(
	__inout a65_tree &tree
	)
{
	a65_token entry;
	a65_tree subtree;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	enumerate_expression_arithmetic_0(subtree);

	entry = a65_lexer::token();
	if(entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_LOGICAL_SHIFT_LEFT)
			|| entry.match(A65_TOKEN_SYMBOL, A65_TOKEN_SYMBOL_LOGICAL_SHIFT_RIGHT)) {
		add_child_subtree(tree, A65_NODE_OPERATOR, entry.id());
		add_child_subtree(tree, subtree);

		if(!a65_lexer::has_next()) {
			A65_THROW_EXCEPTION_INFO("Unterminated expression", "%s", A65_STRING_CHECK(entry.to_string()));
		}

		a65_lexer::move_next();
		enumerate_expression_logical(tree);
		a65_tree::move_parent(tree);
	} else {
		add_child_subtree(tree, subtree);
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_label(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();
	add_child_subtree(tree, A65_NODE_LABEL, entry.id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_pragma(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token();

	switch(entry.subtype()) {
		case A65_TOKEN_PRAGMA_INCLUDE_BINARY:
			enumerate_pragma_include_binary(tree);
			break;
		case A65_TOKEN_PRAGMA_INCLUDE_SOURCE:
			enumerate_pragma_include_source(tree);
			break;
		case A65_TOKEN_PRAGMA_METADATA:
			enumerate_pragma_metadata(tree);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Unsupported pragma", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_pragma_include_binary(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = token();
	add_child_subtree(tree, A65_NODE_PRAGMA, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated pragma", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = token();
	if(!entry.match(A65_TOKEN_LITERAL)) {
		A65_THROW_EXCEPTION_INFO("Expecting literal", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_pragma_include_source(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = token();
	add_child_subtree(tree, A65_NODE_PRAGMA, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated pragma", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = token();
	if(!entry.match(A65_TOKEN_LITERAL)) {
		A65_THROW_EXCEPTION_INFO("Expecting literal", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_pragma_metadata(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = token();
	add_child_subtree(tree, A65_NODE_PRAGMA, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated pragma", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = token();
	if(!entry.match(A65_TOKEN_LITERAL)) {
		A65_THROW_EXCEPTION_INFO("Expecting literal", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(!a65_lexer::has_next()) {
		A65_THROW_EXCEPTION_INFO("Unterminated pragma", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	a65_lexer::move_next();

	entry = token();
	if(!entry.match(A65_TOKEN_SCALAR)) {
		A65_THROW_EXCEPTION_INFO("Expecting scalar", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	add_child(tree, A65_NODE_CONSTANT, entry.id());

	if(a65_lexer::has_next()) {
		a65_lexer::move_next();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
}

void
a65_parser::enumerate_statement_list(
	__inout a65_tree &tree
	)
{
	a65_token entry;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	add_child_subtree(tree, A65_NODE_LIST);

	entry = a65_lexer::token();
	while(entry.match(A65_TOKEN_COMMAND)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_DATA_BYTE)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_DATA_WORD)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_DEFINE)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_IF)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_IF_DEFINE)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ORIGIN)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_RESERVE)
			|| entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_UNDEFINE)
			|| entry.match(A65_TOKEN_LABEL)
			|| entry.match(A65_TOKEN_PRAGMA)) {
		enumerate(tree);
		entry = a65_lexer::token();
	}

	a65_tree::move_parent(tree);

	A65_DEBUG_EXIT();
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
	A65_DEBUG_ENTRY_INFO("Input[%u]=%p (%s)", input.size(), &input, is_path ? "Path" : "Raw");

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

	if(a65_lexer::token().match(A65_TOKEN_BEGIN)) {
		a65_lexer::move_next();
	}

	if(a65_lexer::has_next() && (m_tree_position <= (m_tree.size() - A65_TREE_SENTINEL_COUNT))) {
		a65_tree tree;

		enumerate(tree);

		if(tree.has_root()) {
			tree.move_root();
		}

		add(tree);
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
		result << as_string(entry, A65_TREE_STRING_OFFSET_START);
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
