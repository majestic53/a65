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

#include "../inc/a65_assembler.h"
#include "../inc/a65_utility.h"

a65_assembler::a65_assembler(void) :
		a65_parser(std::string())
{
	A65_DEBUG_ENTRY();

	a65_assembler::clear();

	A65_DEBUG_EXIT();
}

a65_assembler::a65_assembler(
	__in const a65_assembler &other
	) :
		a65_parser(other),
		m_input(other.m_input),
		m_output(other.m_output),
		m_section(other.m_section)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_assembler::~a65_assembler(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT();
}

a65_assembler &
a65_assembler::operator=(
	__in const a65_assembler &other
	)
{
	A65_DEBUG_ENTRY();

	if(this != &other) {
		a65_parser::operator=(other);
		m_input = other.m_input;
		m_output = other.m_output;
		m_section = other.m_section;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

std::string
a65_assembler::as_source(
	__in a65_tree &tree
	) const
{
	uint16_t scalar;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token(tree.node().token());
	result << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
		<< " " << A65_CHARACTER_LITERAL << entry.path() << A65_CHARACTER_LITERAL
		<< " " << entry.line() << std::endl;

	switch(entry.type()) {
		case A65_TOKEN_COMMAND:
			result << as_source_command(tree) << std::endl;
			break;
		case A65_TOKEN_CONSTANT:
			result << A65_TOKEN_CONSTANT_STRING(entry.subtype());
			break;
		case A65_TOKEN_DIRECTIVE:
			result << as_source_directive(tree) << std::endl;
			break;
		case A65_TOKEN_IDENTIFIER:
			result << entry.literal();
			break;
		case A65_TOKEN_LABEL:
			result << entry.literal() << A65_CHARACTER_LABEL << std::endl;
			break;
		case A65_TOKEN_LITERAL:
			result << A65_CHARACTER_LITERAL << entry.literal_formatted() << A65_CHARACTER_LITERAL;
			break;
		case A65_TOKEN_MACRO:
			result << A65_TOKEN_MACRO_STRING(entry.subtype());
			break;
		case A65_TOKEN_PRAGMA:
			result << as_source_pragma(tree) << std::endl;
			break;
		case A65_TOKEN_REGISTER:
			result << A65_TOKEN_REGISTER_STRING(entry.subtype());
			break;
		case A65_TOKEN_SCALAR:

			scalar = entry.scalar();
			if(scalar > UINT8_MAX) {
				result << A65_STRING_HEX(uint16_t, entry.scalar());
			} else {
				result << A65_STRING_HEX(uint8_t, entry.scalar());
			}
			break;
		case A65_TOKEN_SYMBOL:
			result << A65_TOKEN_SYMBOL_STRING(entry.subtype());
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::as_source_command(
	__in a65_tree &tree
	) const
{
	int mode, type;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token(tree.node().token());
	type = entry.subtype();
	mode = entry.mode();

	result << A65_TOKEN_COMMAND_STRING(type);
	if(type == A65_TOKEN_COMMAND_CMD) {
		result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_CURLY_OPEN) << entry.scalar()
			<< A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_CURLY_CLOSE);
	}

	switch(mode) {
		case A65_TOKEN_COMMAND_MODE_ABSOLUTE:
		case A65_TOKEN_COMMAND_MODE_RELATIVE:
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE:
		case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_X:
		case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_Y:
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_X:
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_Y:

			if(!tree.has_child(0)) {
				A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			tree.move_child(0);
			result << " " << as_source_expression(tree);
			tree.move_parent();

			switch(mode) {
				case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_X:
				case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_X:
					result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_SEPERATOR)
						<< " " << A65_TOKEN_REGISTER_STRING(A65_TOKEN_REGISTER_INDEX_X);
					break;
				case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_Y:
				case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_Y:
					result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_SEPERATOR)
						<< " " << A65_TOKEN_REGISTER_STRING(A65_TOKEN_REGISTER_INDEX_Y);
					break;
				default:
					break;
			}
			break;
		case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_INDIRECT:
		case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDIRECT:
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_INDIRECT:
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT:
			result << " " << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_SQUARE_OPEN);

			if(!tree.has_child(0)) {
				A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			tree.move_child(0);
			result << " " << as_source_expression(tree);
			tree.move_parent();

			switch(mode) {
				case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_INDIRECT:
				case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_INDIRECT:
					result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_SEPERATOR)
						<< " " << A65_TOKEN_REGISTER_STRING(A65_TOKEN_REGISTER_INDEX_X);
					break;
				default:
					break;
			}

			result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_SQUARE_CLOSE);
			break;
		case A65_TOKEN_COMMAND_MODE_ACCUMULATOR:
			result << " " << A65_TOKEN_REGISTER_STRING(A65_TOKEN_REGISTER_ACCUMULATOR);
			break;
		case A65_TOKEN_COMMAND_MODE_IMMEDIATE:
			result << " " << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_IMMEDIATE);

			if(!tree.has_child(0)) {
				A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			tree.move_child(0);
			result << " " << as_source_expression(tree);
			tree.move_parent();
			break;
		case A65_TOKEN_COMMAND_MODE_IMPLIED:
			break;
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT_INDEX:
			result << " " << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_SQUARE_OPEN);

			if(!tree.has_child(0)) {
				A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			tree.move_child(0);
			result << " " << as_source_expression(tree);
			tree.move_parent();
			result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_SQUARE_CLOSE)
				<< A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_SEPERATOR)
				<< " " << A65_TOKEN_REGISTER_STRING(A65_TOKEN_REGISTER_INDEX_Y);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::as_source_directive(
	__in a65_tree &tree
	) const
{
	int type;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token(tree.node().token());
	type = entry.subtype();
	result << A65_TOKEN_DIRECTIVE_STRING(type);

	switch(type) {
		case A65_TOKEN_DIRECTIVE_DATA_BYTE:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_DATA_WORD:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_DEFINE:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_IF:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_IF_DEFINE:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_IF_DEFINE_NOT:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_ORIGIN:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_RESERVE:
			// TODO
			break;
		case A65_TOKEN_DIRECTIVE_UNDEFINE:
			// TODO
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::as_source_expression(
	__in a65_tree &tree
	) const
{
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	// TODO: form expression string

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::as_source_pragma(
	__in a65_tree &tree
	) const
{
	int type;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	entry = a65_lexer::token(tree.node().token());
	type = entry.subtype();
	result << A65_TOKEN_PRAGMA_STRING(type);

	switch(type) {
		case A65_TOKEN_PRAGMA_INCLUDE_BINARY:
		case A65_TOKEN_PRAGMA_INCLUDE_SOURCE:
		case A65_TOKEN_PRAGMA_METADATA:

			if(!tree.has_child(0)) {
				A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			tree.move_child(0);
			result << " " << as_source(tree);
			tree.move_parent();

			if(type == A65_TOKEN_PRAGMA_METADATA) {

				if(!tree.has_child(1)) {
					A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				tree.move_child(1);
				result << " " << as_source(tree);
				tree.move_parent();
			}
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

void
a65_assembler::clear(void)
{
	A65_DEBUG_ENTRY();

	a65_parser::reset();
	m_input.clear();
	m_output.clear();
	m_section.clear();

	A65_DEBUG_EXIT();
}

void
a65_assembler::evaluate(void)
{
	A65_DEBUG_ENTRY();

	// TODO: evaluate parser contents
	while(a65_parser::has_next()) {
		a65_tree tree = a65_parser::tree();

		if(!tree.node().match(A65_NODE_BEGIN)
				&& !tree.node().match(A65_NODE_END)) {
			std::cout << /*a65_parser::to_string() << std::endl*/ as_source(tree);
		}

		a65_parser::move_next();
	}
	// ---

	A65_DEBUG_EXIT();
}

void
a65_assembler::form_binary(void)
{
	A65_DEBUG_ENTRY();

	// TODO: form binary file from sectors

	A65_DEBUG_EXIT();
}

void
a65_assembler::form_ihex(void)
{
	A65_DEBUG_ENTRY();

	// TODO: form ihex file sectors

	A65_DEBUG_EXIT();
}

void
a65_assembler::form_listing(void)
{
	A65_DEBUG_ENTRY();

	// TODO: form listing file sectors

	A65_DEBUG_EXIT();
}

void
a65_assembler::run(
	__in_opt const std::string &input,
	__in_opt const std::string &output,
	__in_opt int options
	)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Output[%u]=%p, Options=%u(%x)", input.size(), &input, output.size(), &output, options, options);

	if(!input.empty()) {
		m_input = input;
		a65_parser::load(input);
		a65_assembler::clear();
	}

	if(output.empty()) {

		if(!m_input.empty()) {
			// TODO: form output path from input (add path decomp routine to utility class)
		} else {
			m_output = A65_ASSEMBLER_OUPUT_DEFAULT;
		}
	} else {
		m_output = output;
	}

	preprocess();
	evaluate();

	if(options & A65_BINARY) {
		form_binary();
	}

	if(options & A65_IHEX) {
		form_ihex();
	}

	if(options & A65_LISTING) {
		form_listing();
	}

	A65_DEBUG_EXIT();
}

void
a65_assembler::preprocess(void)
{
	A65_DEBUG_ENTRY();

	// TODO: evaluate directive, pragmas and form new source

	A65_DEBUG_EXIT();
}
