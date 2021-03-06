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

#include <climits>
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
		m_define(other.m_define),
		m_export(other.m_export),
		m_input(other.m_input),
		m_label(other.m_label),
		m_name(other.m_name),
		m_offset(other.m_offset),
		m_origin(other.m_origin),
		m_output(other.m_output),
		m_second_pass(other.m_second_pass),
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
		m_define = other.m_define;
		m_export = other.m_export;
		m_input = other.m_input;
		m_label = other.m_label;
		m_name = other.m_name;
		m_offset = other.m_offset;
		m_origin = other.m_origin;
		m_output = other.m_output;
		m_second_pass = other.m_second_pass;
		m_section = other.m_section;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_assembler::add_define(
	__in const a65_token &token,
	__in uint16_t value
	)
{
	std::string name;
	std::map<std::string, uint16_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Token=%p, Value=%u(%04x)", &token, value, value);

	name = token.literal();

	entry = m_define.find(name);
	if(entry != m_define.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate define", "%s", A65_STRING_CHECK(token.to_string()));
	}

	m_define.insert(std::make_pair(name, value));

	A65_DEBUG_EXIT();
}

void
a65_assembler::add_export(
	__in const a65_token &token
	)
{
	std::string name;
	std::set<std::string>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Token=%p", &token);

	name = token.literal();

	entry = m_export.find(name);
	if(entry != m_export.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate export", "%s", A65_STRING_CHECK(token.to_string()));
	}

	m_export.insert(name);

	A65_DEBUG_EXIT();
}

void
a65_assembler::add_label(
	__in const a65_token &token,
	__in uint16_t origin
	)
{
	std::string name;
	std::map<std::string, uint16_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Token=%p, Origin=%u(%04x)", &token, origin, origin);

	name = token.literal();

	entry = m_label.find(name);
	if(entry != m_label.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate label", "%s", A65_STRING_CHECK(token.to_string()));
	}

	m_label.insert(std::make_pair(name, origin));

	A65_DEBUG_EXIT();
}

void
a65_assembler::add_section(
	__in const std::string &name,
	__in uint16_t origin
	)
{
	std::map<uint16_t, a65_section>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Origin=%u(%04x)", name.size(), A65_STRING_CHECK(name), origin, origin);

	entry = m_section.find(origin);
	if(entry != m_section.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate section", "[%u]%s=%u(%04x)", name.size(), A65_STRING_CHECK(name), origin, origin);
	}

	m_section.insert(std::make_pair(origin, a65_section(name, origin)));

	A65_DEBUG_EXIT();
}

std::string
a65_assembler::build_archive(
	__in const std::vector<std::string> &input,
	__in const std::string &output,
	__in const std::string &name
	)
{
	std::string result;

	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Output[%u]=%s, Name[%u]=%s", input.size(), &input, output.size(), A65_STRING_CHECK(output),
		name.size(), A65_STRING_CHECK(name));

	a65_assembler::clear();

	if(output.empty()) {
		m_output = A65_ASSEMBLER_OUTPUT_PATH_DEFAULT;
	} else {
		m_output = output;
	}

	result = output_archive(name, input);

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.size(), A65_STRING_CHECK(result));
	return result;
}

std::string
a65_assembler::build_object(
	__in const std::string &input,
	__in const std::string &output,
	__in_opt bool header,
	__in_opt bool source
	)
{
	std::string name, result;
	std::stringstream processed;

	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Output[%u]=%p, Header=%x, Source=%x", input.size(), &input, output.size(), &output, header, source);

	m_input = a65_utility::decompose_file_path(input, name);
	a65_parser::load(input);
	a65_assembler::clear();

	if(output.empty()) {

		if(!m_input.empty()) {
			m_output = m_input;
		} else {
			m_output = A65_ASSEMBLER_OUTPUT_PATH_DEFAULT;
		}
	} else {
		m_output = output;
	}

	processed << A65_CHARACTER_COMMENT << " Generated with " << A65 << " Assembler "
			<< A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_NOTICE
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_ASSEMBLER_DIVIDER
		<< std::endl << A65_CHARACTER_COMMENT << " Input: " << input << ", Output: " << m_output
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_ASSEMBLER_DIVIDER
		<< std::endl << preprocess(std::string());

	if(source) {
		output_source(name, processed.str());
	}

	a65_parser::load(processed.str(), false);

	a65_assembler::clear();
	evaluate(name, processed.str());

	m_second_pass = true;
	a65_assembler::clear();
	evaluate(name, processed.str());
	m_second_pass = false;

	result = output_object(name, header);

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.size(), A65_STRING_CHECK(result));
	return result;
}

void
a65_assembler::clear(void)
{
	A65_DEBUG_ENTRY();

	a65_parser::reset();
	m_define.clear();
	m_export.clear();

	if(!m_second_pass) {
		m_label.clear();
	}

	m_name.clear();
	m_offset = 0;
	m_origin = 0;
	m_section.clear();

	A65_DEBUG_EXIT();
}

std::string
a65_assembler::compile(
	__in const std::vector<std::string> &input,
	__in const std::string &output,
	__in const std::string &name,
	__in_opt bool binary,
	__in_opt bool ihex
	)
{
	std::string result;

	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Output[%u]=%s, Name[%u]=%s, Binary=%x, Ihex=%x", input.size(), &input, output.size(),
		A65_STRING_CHECK(output), name.size(), A65_STRING_CHECK(name), binary, ihex);

	a65_assembler::clear();

	if(output.empty()) {
		m_output = A65_ASSEMBLER_OUTPUT_PATH_DEFAULT;
	} else {
		m_output = output;
	}

	result = output_binary(name, input, binary, ihex);

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.size(), A65_STRING_CHECK(result));
	return result;
}

bool
a65_assembler::contains_define(
	__in const std::string &name
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	result = (m_define.find(name) != m_define.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_assembler::contains_export(
	__in const std::string &name
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	result = (m_export.find(name) != m_export.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_assembler::contains_label(
	__in const std::string &name
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	result = (m_label.find(name) != m_label.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

bool
a65_assembler::contains_section(
	__in uint16_t origin
	) const
{
	bool result;

	A65_DEBUG_ENTRY_INFO("Origin=%u(%04x)", origin, origin);

	result = (m_section.find(origin) != m_section.end());

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_assembler::evaluate(
	__in const std::string &name,
	__in const std::string &input
	)
{
	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Input[%u]=%p", name.size(), A65_STRING_CHECK(name), input.size(), &input);

	a65_parser::load(input, false);
	a65_lexer::set_metadata(name);

	while(a65_parser::has_next()) {
		a65_tree tree = a65_parser::tree();

		if(!tree.node().match(A65_NODE_BEGIN)
				&& !tree.node().match(A65_NODE_END)) {
			std::vector<uint8_t> data;

			data = evaluate(*this, tree);
			if(!data.empty()) {

				if(!contains_section(m_origin)) {

					if(m_name.empty()) {
						std::stringstream stream;

						stream << A65_ASSEMBLER_SECTION_NAME_DEFAULT << "_" << A65_STRING_HEX(uint16_t, m_origin);
						m_name = stream.str();
					}

					add_section(m_name, m_origin);
					m_name.clear();
				}

				find_section(m_origin)->second.add(data, tree.id());
				m_offset += data.size();
			}
		}

		a65_parser::move_next();
	}

	A65_DEBUG_EXIT();
}

std::vector<uint8_t>
a65_assembler::evaluate(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	a65_token entry;
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
	switch(entry.type()) {
		case A65_TOKEN_COMMAND:
			result = evaluate_command(parser, tree);
			break;
		case A65_TOKEN_DIRECTIVE:
			result = evaluate_directive(parser, tree);
			break;
		case A65_TOKEN_LABEL:

			if(!m_second_pass) {
				m_name = entry.literal();
				add_label(entry, m_origin + m_offset);
			}
			break;
		case A65_TOKEN_PRAGMA:
			result = evaluate_pragma(parser, tree);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_command(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	int mode, type;
	a65_token entry;
	uint16_t operand;
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
	if(!entry.match(A65_TOKEN_COMMAND)) {
		A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	mode = entry.mode();
	type = entry.subtype();

	if(type != A65_TOKEN_COMMAND_CMD) {
		uint8_t opcode;

		switch(mode) {
			case A65_TOKEN_COMMAND_MODE_ABSOLUTE:

				if(!A65_IS_COMMAND_ABSOLUTE(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ABSOLUTE_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				if((operand <= UINT8_MAX) && A65_IS_COMMAND_ZEROPAGE(entry.subtype())) {
					opcode = A65_COMMAND_ZEROPAGE_OPCODE(type);
					result.push_back(opcode);
					result.push_back(operand);
				} else {
					result.push_back(opcode);
					result.push_back(operand);
					result.push_back(operand >> CHAR_BIT);
				}
				break;
			case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_INDIRECT:

				if(!A65_IS_COMMAND_ABSOLUTE_INDEX_INDIRECT(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ABSOLUTE_INDEX_INDIRECT_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				if((operand <= UINT8_MAX) && A65_IS_COMMAND_ZEROPAGE_INDEX_INDIRECT(entry.subtype())) {
					opcode = A65_COMMAND_ZEROPAGE_INDEX_INDIRECT_OPCODE(type);
					result.push_back(opcode);
					result.push_back(operand);
				} else {
					result.push_back(opcode);
					result.push_back(operand);
					result.push_back(operand >> CHAR_BIT);
				}
				break;
			case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_X:

				if(!A65_IS_COMMAND_ABSOLUTE_INDEX_X(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ABSOLUTE_INDEX_X_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				if((operand <= UINT8_MAX) && A65_IS_COMMAND_ZEROPAGE_INDEX_X(entry.subtype())) {
					opcode = A65_COMMAND_ZEROPAGE_INDEX_X_OPCODE(type);
					result.push_back(opcode);
					result.push_back(operand);
				} else {
					result.push_back(opcode);
					result.push_back(operand);
					result.push_back(operand >> CHAR_BIT);
				}
				break;
			case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_Y:

				if(!A65_IS_COMMAND_ABSOLUTE_INDEX_Y(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ABSOLUTE_INDEX_Y_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				if((operand <= UINT8_MAX) && A65_IS_COMMAND_ZEROPAGE_INDEX_Y(entry.subtype())) {
					opcode = A65_COMMAND_ZEROPAGE_INDEX_Y_OPCODE(type);
					result.push_back(opcode);
					result.push_back(operand);
				} else {
					result.push_back(opcode);
					result.push_back(operand);
					result.push_back(operand >> CHAR_BIT);
				}
				break;
			case A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDIRECT:

				if(!A65_IS_COMMAND_ABSOLUTE_INDIRECT(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ABSOLUTE_INDIRECT_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				if((operand <= UINT8_MAX) && A65_IS_COMMAND_ZEROPAGE_INDIRECT(entry.subtype())) {
					opcode = A65_COMMAND_ZEROPAGE_INDIRECT_OPCODE(type);
					result.push_back(opcode);
					result.push_back(operand);
				} else {
					result.push_back(opcode);
					result.push_back(operand);
					result.push_back(operand >> CHAR_BIT);
				}
				break;
			case A65_TOKEN_COMMAND_MODE_ACCUMULATOR:

				if(!A65_IS_COMMAND_ACCUMULATOR(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ACCUMULATOR_OPCODE(type);
				result.push_back(opcode);
				break;
			case A65_TOKEN_COMMAND_MODE_IMMEDIATE:

				if(!A65_IS_COMMAND_IMMEDIATE(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_IMMEDIATE_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			case A65_TOKEN_COMMAND_MODE_IMPLIED:

				if(!A65_IS_COMMAND_IMPLIED(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_IMPLIED_OPCODE(type);
				result.push_back(opcode);
				break;
			case A65_TOKEN_COMMAND_MODE_RELATIVE:

				if(!A65_IS_COMMAND_RELATIVE(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_RELATIVE_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				if(m_second_pass && ((operand < ((m_origin + m_offset) - UINT8_MAX))
						|| (operand > ((m_origin + m_offset) + UINT8_MAX)))) {
					A65_THROW_EXCEPTION_INFO("Relative jump out-of-range", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				result.push_back(opcode);
				result.push_back(operand - (m_origin + m_offset) - A65_COMMAND_RELATIVE_LENGTH(type));
				break;
			case A65_TOKEN_COMMAND_MODE_ZEROPAGE:

				if(!A65_IS_COMMAND_ZEROPAGE(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ZEROPAGE_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_INDIRECT:

				if(!A65_IS_COMMAND_ZEROPAGE_INDEX_INDIRECT(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ZEROPAGE_INDEX_INDIRECT_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_X:

				if(!A65_IS_COMMAND_ZEROPAGE_INDEX_X(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ZEROPAGE_INDEX_X_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_Y:

				if(!A65_IS_COMMAND_ZEROPAGE_INDEX_Y(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ZEROPAGE_INDEX_Y_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT:

				if(!A65_IS_COMMAND_ZEROPAGE_INDIRECT(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ZEROPAGE_INDIRECT_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT_INDEX:

				if(!A65_IS_COMMAND_ZEROPAGE_INDIRECT_INDEX(type)) {
					A65_THROW_EXCEPTION_INFO("Unsupported addressing mode", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				opcode = A65_COMMAND_ZEROPAGE_INDIRECT_INDEX_OPCODE(type);

				a65_tree::move_child(tree, 0);
				operand = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);

				result.push_back(opcode);
				result.push_back(operand);
				break;
			default:
				A65_THROW_EXCEPTION_INFO("Malformed command tree", "%s", A65_STRING_CHECK(entry.to_string()));
		}
	} else {
		result.push_back(entry.scalar());

		if(tree.node().has_child(0)) {
			a65_tree::move_child(tree, 0);
			operand = evaluate_expression(parser, tree);
			a65_tree::move_parent(tree);

			if(operand <= UINT8_MAX) {
				result.push_back(operand);
			} else {
				result.push_back(operand);
				result.push_back(operand >> CHAR_BIT);
			}
		}
	}

	A65_DEBUG_EXIT();
	return result;
}

bool
a65_assembler::evaluate_condition(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	a65_token entry;
	bool result = false;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	if(tree.node().has_token()) {
		uint16_t left, right;

		a65_tree::move_child(tree, 0);
		left = evaluate_expression(parser, tree);
		a65_tree::move_parent(tree);

		a65_tree::move_child(tree, 1);
		right = evaluate_expression(parser, tree);
		a65_tree::move_parent(tree);

		entry = parser.token(tree.node().token());
		switch(entry.subtype()) {
			case A65_TOKEN_SYMBOL_OPERATOR_EQUALS:
				result = (left == right);
				break;
			case A65_TOKEN_SYMBOL_OPERATOR_GREATER_THAN:
				result = (left > right);
				break;
			case A65_TOKEN_SYMBOL_OPERATOR_GREATER_THAN_EQUALS:
				result = (left >= right);
				break;
			case A65_TOKEN_SYMBOL_OPERATOR_LESS_THAN:
				result = (left < right);
				break;
			case A65_TOKEN_SYMBOL_OPERATOR_LESS_THAN_EQUALS:
				result = (left <= right);
				break;
			case A65_TOKEN_SYMBOL_OPERATOR_NOT_EQUALS:
				result = (left != right);
				break;
			default:
				A65_THROW_EXCEPTION_INFO("Malformed condition tree", "%s", A65_STRING_CHECK(entry.to_string()));
		}
	} else {
		a65_tree::move_child(tree, 0);
		result = (evaluate_expression(parser, tree) != 0);
		a65_tree::move_parent(tree);
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_directive(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	int type;
	a65_token entry;
	uint16_t value = 1;
	bool branch = false;
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());

	type = entry.subtype();
	switch(type) {
		case A65_TOKEN_DIRECTIVE_DATA_BYTE:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!tree.node().match(A65_NODE_LIST)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			for(size_t child = 0; child < tree.node().child_count(); ++child) {
				std::string literal;

				a65_tree::move_child(tree, child);

				if(!tree.node().has_child(0)) {
					A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				a65_tree::move_child(tree, 0);

				entry = parser.token(tree.node().token());
				if((entry.type() == A65_TOKEN_LITERAL) && !tree.node().child_count()) {
					literal = entry.literal();
				}

				a65_tree::move_parent(tree);

				if(!literal.empty()) {
					result.insert(result.end(), literal.begin(), literal.end());
				} else {
					result.push_back(evaluate_expression(parser, tree));
				}

				a65_tree::move_parent(tree);
			}

			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_DATA_WORD:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!tree.node().match(A65_NODE_LIST)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			for(size_t child = 0; child < tree.node().child_count(); ++child) {
				std::string literal;

				a65_tree::move_child(tree, child);

				if(!tree.node().has_child(0)) {
					A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				a65_tree::move_child(tree, 0);

				entry = parser.token(tree.node().token());
				if((entry.type() == A65_TOKEN_LITERAL) && !tree.node().child_count()) {
					literal = entry.literal();
				}

				a65_tree::move_parent(tree);

				if(!literal.empty()) {

					for(std::string::iterator ch = literal.begin(); ch != literal.end(); ++ch) {
						result.push_back(*ch);
						result.push_back(0);
					}
				} else {
					value = evaluate_expression(parser, tree);
					result.push_back(value);
					result.push_back(value >> CHAR_BIT);
				}

				a65_tree::move_parent(tree);
			}

			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_DEFINE:

			if(tree.has_child(1)) {
				a65_tree::move_child(tree, 1);
				value = evaluate_expression(parser, tree);
				a65_tree::move_parent(tree);
			}

			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			add_define(entry, value);
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_EXPORT:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			add_export(entry);
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_IF:
			a65_tree::move_child(tree, 0);
			entry = parser.token(tree.node().token());

			if(!tree.node().match(A65_NODE_CONDITION)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			branch = evaluate_condition(parser, tree);
			a65_tree::move_parent(tree);

			if(branch) {
				a65_tree::move_child(tree, 1);
				result = evaluate_list(parser, tree);
				a65_tree::move_parent(tree);
			} else {

				for(size_t child = 2; child < tree.node().child_count(); ++child) {
					a65_tree::move_child(tree, child);

					entry = parser.token(tree.node().token());
					if(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE_IF)) {
						a65_tree::move_child(tree, 0);
						branch = evaluate_condition(parser, tree);
						a65_tree::move_parent(tree);

						if(branch) {
							a65_tree::move_child(tree, 1);
							result = evaluate_list(parser, tree);
							a65_tree::move_parent(tree);
						}
					} else if(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE)) {
						branch = true;
						a65_tree::move_child(tree, 0);
						result = evaluate_list(parser, tree);
						a65_tree::move_parent(tree);
					} else {
						A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
					}

					a65_tree::move_parent(tree);

					if(branch) {
						break;
					}
				}
			}
			break;
		case A65_TOKEN_DIRECTIVE_IF_DEFINE:
		case A65_TOKEN_DIRECTIVE_IF_DEFINE_NOT:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			branch = contains_define(entry.literal());
			a65_tree::move_parent(tree);

			if(type == A65_TOKEN_DIRECTIVE_IF_DEFINE_NOT) {
				branch = !branch;
			}

			if(branch) {
				a65_tree::move_child(tree, 1);
				result = evaluate_list(parser, tree);
				a65_tree::move_parent(tree);
			} else if(tree.has_child(2)) {
				a65_tree::move_child(tree, 2);

				if(!tree.has_child(0)) {
					A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				a65_tree::move_child(tree, 0);
				result = evaluate_list(parser, tree);
				a65_tree::move_parent(tree);
				a65_tree::move_parent(tree);
			}
			break;
		case A65_TOKEN_DIRECTIVE_ORIGIN:
			a65_tree::move_child(tree, 0);
			m_origin = evaluate_expression(parser, tree);
			m_offset = 0;
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_RESERVE:
			a65_tree::move_child(tree, 0);
			result.resize(evaluate_expression(parser, tree), A65_ASSEMBLER_FILL_RESERVE);
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_UNDEFINE:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			remove_define(entry);
			a65_tree::move_parent(tree);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result;
}

uint16_t
a65_assembler::evaluate_expression(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	a65_token entry;
	std::string literal;
	uint16_t left, result = 0, right;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());

	switch(tree.node().type()) {
		case A65_NODE_EXPRESSION:
			a65_tree::move_child(tree, 0);
			result = evaluate_expression(parser, tree);
			a65_tree::move_parent(tree);
			break;
		case A65_NODE_CONSTANT:

			switch(entry.type()) {
				case A65_TOKEN_CONSTANT:

					switch(entry.subtype()) {
						case A65_TOKEN_CONSTANT_FALSE:
							result = false;
							break;
						case A65_TOKEN_CONSTANT_NULL:
							result = (uint16_t)NULL;
							break;
						case A65_TOKEN_CONSTANT_TRUE:
							result = true;
							break;
						default:
							A65_THROW_EXCEPTION_INFO("Malformed expression tree", "%s",
											A65_STRING_CHECK(entry.to_string()));
					}
					break;
				case A65_TOKEN_IDENTIFIER:
					literal = entry.literal();

					if(contains_define(literal)) {
						result = find_define(literal)->second;
					} else if(contains_label(literal)) {
						result = find_label(literal)->second;
					} else if(!m_second_pass) {
						result = UINT16_MAX;
					} else {
						A65_THROW_EXCEPTION_INFO("Undefined operand", "%s", A65_STRING_CHECK(entry.to_string()));
					}
					break;
				case A65_TOKEN_LITERAL:
					literal = entry.literal();

					if(tree.node().has_child(0)) {
						uint16_t index;

						a65_tree::move_child(tree, 0);
						index = evaluate_expression(parser, tree);
						a65_tree::move_parent(tree);

						if(index >= literal.size()) {
							A65_THROW_EXCEPTION_INFO("Literal out-of-range", "%s", A65_STRING_CHECK(entry.to_string()));
						}

						result = literal.at(index);
					} else {
						result = literal.front();
					}
					break;
				case A65_TOKEN_SCALAR:
					result = entry.scalar();
					break;
				default:
					A65_THROW_EXCEPTION_INFO("Malformed expression tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}
			break;
		case A65_NODE_MACRO:
			a65_tree::move_child(tree, 0);
			result = evaluate_expression(parser, tree);
			a65_tree::move_parent(tree);

			switch(entry.subtype()) {
				case A65_TOKEN_MACRO_HIGH:
					result >>= CHAR_BIT;
					break;
				case A65_TOKEN_MACRO_LOW:
					result &= UINT8_MAX;
					break;
				default:
					A65_THROW_EXCEPTION_INFO("Malformed expression tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}
			break;
		case A65_NODE_OPERATOR:
			a65_tree::move_child(tree, 0);
			left = evaluate_expression(parser, tree);
			a65_tree::move_parent(tree);

			a65_tree::move_child(tree, 1);
			right = evaluate_expression(parser, tree);
			a65_tree::move_parent(tree);

			switch(entry.subtype()) {
				case A65_TOKEN_SYMBOL_ARITHMETIC_ADDITION:
					result = (left + right);
					break;
				case A65_TOKEN_SYMBOL_ARITHMETIC_DIVIDE:
					result = (left / right);
					break;
				case A65_TOKEN_SYMBOL_ARITHMETIC_MODULUS:
					result = (left % right);
					break;
				case A65_TOKEN_SYMBOL_ARITHMETIC_MULTIPLY:
					result = (left * right);
					break;
				case A65_TOKEN_SYMBOL_ARITHMETIC_SUBTRACTION:
					result = (left - right);
					break;
				case A65_TOKEN_SYMBOL_BINARY_AND:
					result = (left & right);
					break;
				case A65_TOKEN_SYMBOL_BINARY_OR:
					result = (left | right);
					break;
				case A65_TOKEN_SYMBOL_BINARY_XOR:
					result = (left ^ right);
					break;
				case A65_TOKEN_SYMBOL_LOGICAL_AND:
					result = (left && right);
					break;
				case A65_TOKEN_SYMBOL_LOGICAL_OR:
					result = (left || right);
					break;
				case A65_TOKEN_SYMBOL_LOGICAL_SHIFT_LEFT:
					result = (left << right);
					break;
				case A65_TOKEN_SYMBOL_LOGICAL_SHIFT_RIGHT:
					result = (left >> right);
					break;
				default:
					A65_THROW_EXCEPTION_INFO("Malformed expression tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}
			break;
		case A65_NODE_UNARY:
			a65_tree::move_child(tree, 0);
			result = evaluate_expression(parser, tree);
			a65_tree::move_parent(tree);

			switch(entry.subtype()) {
				case A65_TOKEN_SYMBOL_UNARY_NEGATION:
					result = ~result;
					break;
				case A65_TOKEN_SYMBOL_UNARY_NOT:
					result = !result;
					break;
				default:
					A65_THROW_EXCEPTION_INFO("Malformed expression tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed expression tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT_INFO("Result=%u(%04x)", result, result);
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_list(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	for(size_t child = 0; child < tree.node().child_count(); ++child) {
		std::vector<uint8_t> data;

		a65_tree::move_child(tree, child);
		data = evaluate(parser, tree);
		a65_tree::move_parent(tree);

		if(!data.empty()) {
			result.insert(result.end(), data.begin(), data.end());
		}
	}

	A65_DEBUG_EXIT();
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_pragma(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	uint16_t line;
	a65_token entry;
	std::string path;
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
	switch(entry.subtype()) {
		case A65_TOKEN_PRAGMA_INCLUDE_BINARY:
		case A65_TOKEN_PRAGMA_INCLUDE_SOURCE:
			A65_THROW_EXCEPTION_INFO("Disallowed pragma", "%s", A65_STRING_CHECK(entry.to_string()));
			break;
		case A65_TOKEN_PRAGMA_METADATA:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_LITERAL)) {
				A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			path = entry.literal();
			a65_tree::move_parent(tree);

			a65_tree::move_child(tree, 1);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_SCALAR)) {
				A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			line = entry.scalar();
			a65_tree::move_parent(tree);
			a65_lexer::set_metadata(path, line);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result;
}

std::map<std::string, uint16_t>::iterator
a65_assembler::find_define(
	__in const std::string &name
	)
{
	std::map<std::string, uint16_t>::iterator result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	result = m_define.find(name);
	if(result == m_define.end()) {
		A65_THROW_EXCEPTION_INFO("Define not found", "%s", A65_STRING_CHECK(name));
	}

	A65_DEBUG_EXIT_INFO("Result={[%u]%s, %u(%04x)}", result->first.size(), A65_STRING_CHECK(result->first), result->second, result->second);
	return result;
}

std::map<std::string, uint16_t>::iterator
a65_assembler::find_label(
	__in const std::string &name
	)
{
	std::map<std::string, uint16_t>::iterator result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	result = m_label.find(name);
	if(result == m_label.end()) {
		A65_THROW_EXCEPTION_INFO("Label not found", "%s", A65_STRING_CHECK(name));
	}

	A65_DEBUG_EXIT_INFO("Result={[%u]%s, %u(%04x)}", result->first.size(), A65_STRING_CHECK(result->first), result->second, result->second);
	return result;
}

std::map<uint16_t, a65_section>::iterator
a65_assembler::find_section(
	__in uint16_t origin
	)
{
	std::map<uint16_t, a65_section>::iterator result;

	A65_DEBUG_ENTRY_INFO("Origin=%u(%04x), Name[%u]=%s", origin, origin);

	result = m_section.find(origin);
	if(result == m_section.end()) {
		A65_THROW_EXCEPTION_INFO("Section not found", "%u(%04x)", origin, origin);
	}

	A65_DEBUG_EXIT_INFO("Result={%u(%04x), %p}", result->first, result->first, &result->second);
	return result;
}

std::string
a65_assembler::form_ihex(
	__in int type,
	__in_opt uint16_t origin,
	__in_opt const std::vector<uint8_t> &data
	)
{
	uint16_t checksum;
	std::stringstream result;
	std::vector<uint8_t>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Origin=%u(%04x), Data[%u]=%p", type, A65_IHEX_STRING(type), origin, origin, data.size(), &data);

	switch(type) {
		case A65_IHEX_END:

			if(!data.empty()) {
				A65_THROW_EXCEPTION_INFO("Data too large", "%u (max=%u)", data.size(), 0);
			}
		case A65_IHEX_DATA:

			if(data.size() > UINT8_MAX) {
				A65_THROW_EXCEPTION_INFO("Data too large", "%u (max=%u)", data.size(), UINT8_MAX);
			}

			result << A65_IHEX_CHARACTER << A65_STRING_HEX(uint8_t, data.size()) << A65_STRING_HEX(uint16_t, origin)
				<< A65_STRING_HEX(uint8_t, type);

			checksum = (data.size() + ((origin >> CHAR_BIT) & UINT8_MAX) + (origin & UINT8_MAX) + (type & UINT8_MAX));

			for(entry = data.begin(); entry != data.end(); ++entry) {
				result << A65_STRING_HEX(uint8_t, *entry);
				checksum += *entry;
			}

			result << A65_STRING_HEX(uint8_t, ~(checksum & UINT8_MAX) + 1);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Unsupported ihex type", "%u(%x)", type, type);
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::output_archive(
	__in const std::string &name,
	__in const std::vector<std::string> &input
	)
{
	std::stringstream result;
	a65_archive archive;
	std::vector<a65_object> objects;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Input[%u]=%p", name.size(), A65_STRING_CHECK(name), input.size(), &input);

	result << m_output;

	if(result.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		result << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		result << A65_ASSEMBLER_OUTPUT_ARCHIVE_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		result << name;
	}

	result << A65_ASSEMBLER_OUTPUT_ARCHIVE_EXTENSION;

	if(!input.empty()) {

		for(std::vector<std::string>::const_iterator entry = input.begin(); entry != input.end(); ++entry) {
			objects.push_back(a65_object(*entry));
		}

		archive.import(objects);
	}

	archive.write(result.str());

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

std::string
a65_assembler::output_binary(
	__in const std::string &name,
	__in const std::vector<std::string> &input,
	__in_opt bool binary,
	__in_opt bool ihex
	)
{
	std::stringstream result;
	std::vector<uint8_t> data;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Input[%u]=%p, Binary=%x, Ihex=%x", name.size(), A65_STRING_CHECK(name), input.size(), &input, binary,
		ihex);

	result << m_output;

	if(result.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		result << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		result << A65_ASSEMBLER_OUTPUT_BINARY_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		result << name;
	}

	result << A65_ASSEMBLER_OUTPUT_BINARY_EXTENSION;

	if(!input.empty()) {
		std::vector<a65_object> object;
		std::vector<a65_object>::iterator object_entry;
		std::vector<std::string> archive_file, object_file;
		std::vector<std::string>::const_iterator file_entry;

		data.resize(UINT16_MAX + 1, A65_ASSEMBLER_FILL);

		for(file_entry = input.begin(); file_entry != input.end(); ++file_entry) {
			size_t dot = file_entry->find_last_of(A65_EXTENSION);

			if(dot != std::string::npos) {
				std::string extension = file_entry->substr(dot);

				if(extension == A65_ASSEMBLER_OUTPUT_ARCHIVE_EXTENSION) {
					archive_file.push_back(*file_entry);
				} else if(extension == A65_ASSEMBLER_OUTPUT_OBJECT_EXTENSION) {
					object_file.push_back(*file_entry);
				} else {
					A65_THROW_EXCEPTION_INFO("Unsupported file", "%s", A65_STRING_CHECK(*file_entry));
				}
			}
		}

		for(file_entry = archive_file.begin(); file_entry != archive_file.end(); ++file_entry) {
			a65_archive archive(*file_entry);

			for(size_t iter = 0; iter < archive.count(); ++iter) {
				object.push_back(a65_object());
				archive.object(iter, object.back());
			}
		}

		for(file_entry = object_file.begin(); file_entry != object_file.end(); ++file_entry) {
			object.push_back(a65_object(*file_entry));
		}

		if(binary) {

			for(object_entry = object.begin(); object_entry != object.end(); ++object_entry) {
				size_t position = 0;

				while(object_entry->contains_section(position)) {
					uint16_t origin;
					std::vector<uint8_t> object_data;

					origin = object_entry->section(position, object_data);

					for(size_t offset = 0; offset < object_data.size(); ++offset) {
						data.at(origin + offset) = object_data.at(offset);
					}

					++position;
				}
			}

			a65_utility::write_file(result.str(), data);
		}

		if(ihex) {
			output_binary_ihex(name, object);
		}
	}

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

std::string
a65_assembler::output_binary_ihex(
	__in const std::string &name,
	__in const std::vector<a65_object> &object
	)
{
	std::stringstream result, source;
	std::vector<a65_object>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Object[%u]=%p", name.size(), A65_STRING_CHECK(name), object.size(), &object);

	result << m_output;

	if(result.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		result << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		result << A65_ASSEMBLER_OUTPUT_IHEX_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		result << name;
	}

	result << A65_ASSEMBLER_OUTPUT_IHEX_EXTENSION;

	for(entry = object.begin(); entry != object.end(); ++entry) {
		size_t position = 0;

		while(entry->contains_section(position)) {
			uint16_t origin;
			std::vector<uint8_t> data, subdata;

			origin = entry->section(position, data);

			for(size_t offset = 0; offset < data.size(); ++offset) {

				if(!(offset % A65_IHEX_LENGTH) && !subdata.empty()) {
					source << form_ihex(A65_IHEX_DATA, origin, subdata) << std::endl;
					origin += subdata.size();
					subdata.clear();
				}

				subdata.push_back(data.at(offset));
			}

			if(!subdata.empty()) {
				source << form_ihex(A65_IHEX_DATA, origin, subdata) << std::endl;
				subdata.clear();
			}

			++position;
		}
	}

	source << form_ihex(A65_IHEX_END);

	a65_utility::write_file(result.str(), source.str());

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

std::string
a65_assembler::output_object(
	__in const std::string &name,
	__in_opt bool header
	)
{
	a65_object object;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Header=%x", name.size(), A65_STRING_CHECK(name), header);

	result << m_output;

	if(result.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		result << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		result << A65_ASSEMBLER_OUTPUT_OBJECT_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		result << name;
	}

	result << A65_ASSEMBLER_OUTPUT_OBJECT_EXTENSION;
	object.import(m_section);
	object.write(result.str());

	if(header) {
		output_object_header(result.str(), object);
	}

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

std::string
a65_assembler::output_object_header(
	__in const std::string &name,
	__in const a65_object &object
	)
{
	std::stringstream result, source;
	std::set<std::string>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Object=%p", name.size(), A65_STRING_CHECK(name), &object);

	result << name << A65_ASSEMBLER_OUTPUT_SOURCE_EXTENSION;

	source << A65_CHARACTER_COMMENT << " Generated with " << A65 << " Assembler "
			<< A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_NOTICE
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_ASSEMBLER_DIVIDER
		<< std::endl << A65_CHARACTER_COMMENT << " Object: " << name << ", Output: " << result.str()
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_ASSEMBLER_DIVIDER << std::endl
		<< std::endl << A65_CHARACTER_COMMENT << " Exports:" << std::endl;

	for(entry = m_export.begin(); entry != m_export.end(); ++entry) {
		std::map<std::string, uint16_t>::iterator label = find_label(*entry);

		if(entry != m_export.begin()) {
			source << std::endl;
		}

		source << std::endl << label->first << A65_CHARACTER_LABEL << " " << A65_TOKEN_COMMAND_STRING(A65_TOKEN_COMMAND_JSR)
			<< " " << A65_STRING_HEX(uint16_t, label->second);
	}

	a65_utility::write_file(result.str(), source.str());

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

std::string
a65_assembler::output_source(
	__in const std::string &name,
	__in const std::string &source
	)
{
	std::string line;
	std::stringstream input, output, result;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Source[%u]=%p", name.size(), A65_STRING_CHECK(name), source.size(), &source);

	result << m_output;

	if(result.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		result << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		result << A65_ASSEMBLER_OUTPUT_SOURCE_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		result << name;
	}

	result << A65_ASSEMBLER_OUTPUT_SOURCE_EXTENSION;

	input << source;
	while(std::getline(input, line)) {

		if(line.front() == A65_ASSEMBLER_CHARACTER_METADATA) {
			continue;
		}

		if(!output.str().empty()) {
			output << std::endl;
		}

		output << line;
	}

	a65_utility::write_file(result.str(), output.str());

	A65_DEBUG_EXIT_INFO("Result[%u]=%s", result.str().size(), A65_STRING_CHECK(result.str()));
	return result.str();
}

std::string
a65_assembler::preprocess(
	__in_opt const std::string &input
	)
{
	std::string name;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Input[%u]=%p", input.size(), &input);

	if(!input.empty()) {
		m_input = a65_utility::decompose_file_path(input, name);
		a65_parser::load(input);
	} else {
		a65_parser::reset();
	}

	while(a65_parser::has_next()) {
		a65_tree tree = a65_parser::tree();

		if(!tree.node().match(A65_NODE_BEGIN)
				&& !tree.node().match(A65_NODE_END)) {
			a65_token entry;

			entry = a65_parser::token(tree.node().token());
			result << std::endl << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
					<< " " << A65_CHARACTER_LITERAL << entry.path() << A65_CHARACTER_LITERAL
					<< " " << entry.line()
				<< std::endl << preprocess(*this, tree);
		}

		a65_parser::move_next();
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::preprocess(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	uint16_t scalar;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
	switch(entry.type()) {
		case A65_TOKEN_COMMAND:
			result << A65_ASSEMBLER_CHARACTER_TAB << preprocess_command(parser, tree);
			break;
		case A65_TOKEN_CONSTANT:
			result << A65_TOKEN_CONSTANT_STRING(entry.subtype());
			break;
		case A65_TOKEN_DIRECTIVE:
			result << A65_ASSEMBLER_CHARACTER_TAB << preprocess_directive(parser, tree);
			break;
		case A65_TOKEN_IDENTIFIER:
			result << entry.literal();
			break;
		case A65_TOKEN_LABEL:
			result << entry.literal() << A65_CHARACTER_LABEL;
			break;
		case A65_TOKEN_LITERAL:
			result << A65_CHARACTER_LITERAL << entry.literal_formatted() << A65_CHARACTER_LITERAL;

			if(tree.node().has_child(0)) {
				a65_tree::move_child(tree, 0);
				result << " " << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_CURLY_OPEN)
					<< preprocess_expression(parser, tree)
					<< A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_CURLY_CLOSE);
				a65_tree::move_parent(tree);
			}
			break;
		case A65_TOKEN_MACRO:
			result << A65_TOKEN_MACRO_STRING(entry.subtype())
				<< A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_PARENTHESIS_OPEN);
			a65_tree::move_child(tree, 0);
			result << preprocess_expression(parser, tree);
			a65_tree::move_parent(tree);
			result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_PARENTHESIS_CLOSE);
			break;
		case A65_TOKEN_PRAGMA:
			result << A65_ASSEMBLER_CHARACTER_TAB << preprocess_pragma(parser, tree);
			break;
		case A65_TOKEN_REGISTER:
			result << A65_TOKEN_REGISTER_STRING(entry.subtype());
			break;
		case A65_TOKEN_SCALAR:

			scalar = entry.scalar();
			if(scalar > UINT8_MAX) {
				result << A65_CHARACTER_ZERO << A65_CHARACTER_DIGIT_HEXIDECIMAL << A65_STRING_HEX(uint16_t, entry.scalar());
			} else {
				result << A65_CHARACTER_ZERO << A65_CHARACTER_DIGIT_HEXIDECIMAL << A65_STRING_HEX(uint8_t, entry.scalar());
			}
			break;
		case A65_TOKEN_SYMBOL:

			switch(tree.node().type()) {
				case A65_NODE_OPERATOR:
					a65_tree::move_child(tree, 0);
					result << preprocess_expression(*this, tree);
					a65_tree::move_parent(tree);
					result << " " << A65_TOKEN_SYMBOL_STRING(entry.subtype()) << " ";
					a65_tree::move_child(tree, 1);
					result << preprocess_expression(*this, tree);
					a65_tree::move_parent(tree);
					break;
				case A65_NODE_UNARY:
					result << A65_TOKEN_SYMBOL_STRING(entry.subtype());
					a65_tree::move_child(tree, 0);
					result << preprocess_expression(parser, tree);
					a65_tree::move_parent(tree);
					break;
				default:
					result << A65_TOKEN_SYMBOL_STRING(entry.subtype());
					break;
			}
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::preprocess_command(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	int mode, type;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
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
			a65_tree::move_child(tree, 0);
			result << " " << preprocess_expression(parser, tree);
			a65_tree::move_parent(tree);

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

			a65_tree::move_child(tree, 0);
			result << preprocess_expression(parser, tree);
			a65_tree::move_parent(tree);

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

			a65_tree::move_child(tree, 0);
			result << preprocess_expression(parser, tree);
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_COMMAND_MODE_IMPLIED:
			break;
		case A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT_INDEX:
			result << " " << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_BRACE_SQUARE_OPEN);

			a65_tree::move_child(tree, 0);
			result << preprocess_expression(parser, tree);
			a65_tree::move_parent(tree);
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
a65_assembler::preprocess_condition(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	if(tree.node().has_token()) {
		a65_token entry = parser.token(tree.node().token());

		a65_tree::move_child(tree, 0);
		result << preprocess_expression(parser, tree);
		a65_tree::move_parent(tree);
		result << " " << A65_TOKEN_SYMBOL_STRING(entry.subtype()) << " ";
		a65_tree::move_child(tree, 1);
		result << preprocess_expression(parser, tree);
		a65_tree::move_parent(tree);
	} else {
		a65_tree::move_child(tree, 0);
		result << preprocess_expression(parser, tree);
		a65_tree::move_parent(tree);
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::preprocess_directive(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	int type;
	size_t child;
	a65_token entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
	type = entry.subtype();
	result << A65_TOKEN_DIRECTIVE_STRING(type);

	switch(type) {
		case A65_TOKEN_DIRECTIVE_DATA_BYTE:
		case A65_TOKEN_DIRECTIVE_DATA_WORD:
			a65_tree::move_child(tree, 0);

			for(child = 0; child < tree.node().child_count(); ++child) {
				a65_tree::move_child(tree, child);

				if(child) {
					result << ",";
				}

				result << " " << preprocess_expression(parser, tree);
				a65_tree::move_parent(tree);
			}

			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_DEFINE:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			result << " " << entry.literal();
			a65_tree::move_parent(tree);

			if(tree.has_child(1)) {
				a65_tree::move_child(tree, 1);
				result << " " << preprocess_expression(parser, tree);
				a65_tree::move_parent(tree);
			}
			break;
		case A65_TOKEN_DIRECTIVE_EXPORT:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			result << " " << entry.literal();
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_IF:
			a65_tree::move_child(tree, 0);
			result << " " << preprocess_condition(parser, tree);
			a65_tree::move_parent(tree);

			a65_tree::move_child(tree, 1);

			for(child = 0; child < tree.node().child_count(); ++child) {
				a65_tree::move_child(tree, child);
				result << std::endl << preprocess(*this, tree);
				a65_tree::move_parent(tree);
			}

			a65_tree::move_parent(tree);

			child = 2;
			while(tree.has_child(child)) {
				size_t subchild;

				a65_tree::move_child(tree, child);

				entry = parser.token(tree.node().token());
				if(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE_IF)) {
					result << std::endl << A65_TOKEN_DIRECTIVE_STRING(A65_TOKEN_DIRECTIVE_ELSE_IF);
					a65_tree::move_child(tree, 0);
					result << " " << preprocess_condition(parser, tree);
					a65_tree::move_parent(tree);

					a65_tree::move_child(tree, 1);

					for(subchild = 0; subchild < tree.node().child_count(); ++subchild) {
						a65_tree::move_child(tree, subchild);
						result << std::endl << preprocess(*this, tree);
						a65_tree::move_parent(tree);
					}

					a65_tree::move_parent(tree);
				} else if(entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE)) {
					result << std::endl << A65_TOKEN_DIRECTIVE_STRING(A65_TOKEN_DIRECTIVE_ELSE);
					a65_tree::move_child(tree, 0);

					for(subchild = 0; subchild < tree.node().child_count(); ++subchild) {
						a65_tree::move_child(tree, subchild);
						result << std::endl << preprocess(*this, tree);
						a65_tree::move_parent(tree);
					}

					a65_tree::move_parent(tree);
				} else {
					A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				a65_tree::move_parent(tree);
				++child;
			}

			result << std::endl << A65_TOKEN_DIRECTIVE_STRING(A65_TOKEN_DIRECTIVE_END);
			break;
		case A65_TOKEN_DIRECTIVE_IF_DEFINE:
		case A65_TOKEN_DIRECTIVE_IF_DEFINE_NOT:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			result << " " << entry.literal();
			a65_tree::move_parent(tree);
			a65_tree::move_child(tree, 1);

			for(child = 0; child < tree.node().child_count(); ++child) {
				a65_tree::move_child(tree, child);
				result << std::endl << preprocess(*this, tree);
				a65_tree::move_parent(tree);
			}

			a65_tree::move_parent(tree);

			if(tree.has_child(2)) {
				a65_tree::move_child(tree, 2);

				entry = parser.token(tree.node().token());
				if(!entry.match(A65_TOKEN_DIRECTIVE, A65_TOKEN_DIRECTIVE_ELSE)) {
					A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
				}

				result << std::endl << A65_TOKEN_DIRECTIVE_STRING(A65_TOKEN_DIRECTIVE_ELSE);
				a65_tree::move_child(tree, 0);

				for(child = 0; child < tree.node().child_count(); ++child) {
					a65_tree::move_child(tree, child);
					result << std::endl << preprocess(*this, tree);
					a65_tree::move_parent(tree);
				}

				a65_tree::move_parent(tree);
				a65_tree::move_parent(tree);
			}

			result << std::endl << A65_TOKEN_DIRECTIVE_STRING(A65_TOKEN_DIRECTIVE_END);
			break;
		case A65_TOKEN_DIRECTIVE_ORIGIN:
		case A65_TOKEN_DIRECTIVE_RESERVE:
			a65_tree::move_child(tree, 0);
			result << " " << preprocess_expression(parser, tree);
			a65_tree::move_parent(tree);
			break;
		case A65_TOKEN_DIRECTIVE_UNDEFINE:
			a65_tree::move_child(tree, 0);

			entry = parser.token(tree.node().token());
			if(!entry.match(A65_TOKEN_IDENTIFIER)) {
				A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
			}

			result << " " << entry.literal();
			a65_tree::move_parent(tree);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed directive tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::preprocess_expression(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	a65_node entry;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = tree.node();
	switch(entry.type()) {
		case A65_NODE_EXPRESSION:
			a65_tree::move_child(tree, 0);

			entry = tree.node();
			switch(entry.type()) {
				case A65_NODE_EXPRESSION:
					result << A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_PARENTHESIS_OPEN)
						<< preprocess_expression(parser, tree)
						<< A65_TOKEN_SYMBOL_STRING(A65_TOKEN_SYMBOL_PARENTHESIS_CLOSE);
					break;
				default:
					result << preprocess(*this, tree);
					break;
			}

			a65_tree::move_parent(tree);
			break;
		default:
			result << preprocess(*this, tree);
			break;
	}

	A65_DEBUG_EXIT();
	return result.str();
}

std::string
a65_assembler::preprocess_pragma(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	int type;
	a65_token entry;
	a65_assembler assembler;
	std::stringstream path, result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	entry = parser.token(tree.node().token());
	type = entry.subtype();

	switch(type) {
		case A65_TOKEN_PRAGMA_INCLUDE_BINARY:

			try {
				size_t size;
				std::string data;

				a65_tree::move_child(tree, 0);
				entry = parser.token(tree.node().token());
				path << m_input << A65_ASSEMBLER_OUTPUT_SEPERATOR << entry.literal();
				a65_tree::move_parent(tree);

				size = a65_utility::read_file(path.str(), data);
				if(size) {
					result << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
						<< " " << A65_CHARACTER_LITERAL << path.str() << A65_CHARACTER_LITERAL << " " << A65_CHARACTER_ZERO
						<< std::endl << A65_TOKEN_DIRECTIVE_STRING(A65_TOKEN_DIRECTIVE_DATA_BYTE);

					for(std::string::iterator ch = data.begin(); ch != data.end(); ++ch) {

						if(ch != data.begin()) {
							result << ",";
						}

						result << " " << A65_CHARACTER_ZERO << A65_CHARACTER_DIGIT_HEXIDECIMAL << A65_STRING_HEX(uint8_t, *ch);
					}

					entry = parser.token(tree.node().token());
					result << std::endl << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
						<< " " << A65_CHARACTER_LITERAL << entry.path() << A65_CHARACTER_LITERAL
						<< " " << (entry.line() + 1);
				}
			} catch(...) {
				A65_THROW_EXCEPTION_INFO("Include file not found", "%s", A65_STRING_CHECK(entry.to_string()));
			}
			break;
		case A65_TOKEN_PRAGMA_INCLUDE_SOURCE:
			a65_tree::move_child(tree, 0);
			entry = parser.token(tree.node().token());
			path << m_input << A65_ASSEMBLER_OUTPUT_SEPERATOR << entry.literal();
			a65_tree::move_parent(tree);

			result << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
				<< " " << A65_CHARACTER_LITERAL << path.str() << A65_CHARACTER_LITERAL << " " << A65_CHARACTER_ZERO
				<< assembler.preprocess(path.str());

			entry = parser.token(tree.node().token());
			result << std::endl << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
				<< " " << A65_CHARACTER_LITERAL << entry.path() << A65_CHARACTER_LITERAL
				<< " " << (entry.line() + 1);
			break;
		case A65_TOKEN_PRAGMA_METADATA:
			A65_THROW_EXCEPTION_INFO("Disallowed pragma", "%s", A65_STRING_CHECK(entry.to_string()));
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Malformed pragma tree", "%s", A65_STRING_CHECK(entry.to_string()));
	}

	A65_DEBUG_EXIT();
	return result.str();
}

void
a65_assembler::remove_define(
	__in const a65_token &token
	)
{
	std::map<std::string, uint16_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Token=%p", &token);

	entry = m_define.find(token.literal());
	if(entry == m_define.end()) {
		A65_THROW_EXCEPTION_INFO("Define not found", "%s", A65_STRING_CHECK(token.to_string()));
	}

	m_define.erase(entry);

	A65_DEBUG_EXIT();
}
