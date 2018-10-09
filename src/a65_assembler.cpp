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
#include "../inc/a65_object.h"
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
		m_input(other.m_input),
		m_label(other.m_label),
		m_name(other.m_name),
		m_origin(other.m_origin),
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
		m_define = other.m_define;
		m_input = other.m_input;
		m_label = other.m_label;
		m_name = other.m_name;
		m_origin = other.m_origin;
		m_output = other.m_output;
		m_section = other.m_section;
	}

	A65_DEBUG_EXIT_INFO("Result=%p", this);
	return *this;
}

void
a65_assembler::add_define(
	__in const std::string &name,
	__in uint16_t value
	)
{
	std::map<std::string, uint16_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Value=%u(%04x)", name.size(), A65_STRING_CHECK(name), value, value);

	entry = m_define.find(name);
	if(entry != m_define.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate define", "[%u]%s=%u(%04x)", name.size(), A65_STRING_CHECK(name), value, value);
	}

	m_define.insert(std::make_pair(name, value));

	A65_DEBUG_EXIT();
}

void
a65_assembler::add_label(
	__in const std::string &name,
	__in uint16_t origin
	)
{
	std::map<std::string, uint16_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Origin=%u(%04x)", name.size(), A65_STRING_CHECK(name), origin, origin);

	entry = m_label.find(name);
	if(entry != m_label.end()) {
		A65_THROW_EXCEPTION_INFO("Duplicate label", "[%u]%s=%u(%04x)", name.size(), A65_STRING_CHECK(name), origin, origin);
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

void
a65_assembler::clear(void)
{
	A65_DEBUG_ENTRY();

	a65_parser::reset();
	m_define.clear();
	m_label.clear();
	m_name.clear();
	m_origin = 0;
	m_section.clear();

	A65_DEBUG_EXIT();
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
	__in const std::string &input,
	__in_opt bool verbose
	)
{
	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Verbose=%x", input.size(), &input, verbose);

	if(verbose) {
		std::cout << std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl << A65_ASSEMBLER_SECTION_EVALUATE
			<< std::endl << A65_ASSEMBLER_DIVIDER;
	}

	a65_parser::load(input, false);

	while(a65_parser::has_next()) {
		a65_tree tree = a65_parser::tree();

		if(!tree.node().match(A65_NODE_BEGIN)
				&& !tree.node().match(A65_NODE_END)) {
			std::vector<uint8_t> data;

// TODO
//std::cout << a65_parser::as_string(tree, 0) << std::endl;
// ---

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
			}
		}

		a65_parser::move_next();
	}

	if(verbose) {
		// TODO: write information/statistics

		std::cout << std::endl << A65_ASSEMBLER_SECTION_DONE << std::endl;
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
			m_name = entry.literal();
			add_label(m_name, m_origin);
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
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	// TODO: evaluate command

	A65_DEBUG_EXIT();
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_directive(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	// TODO: evaluate directive

	A65_DEBUG_EXIT();
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_expression(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	// TODO: evaluate pragma

	A65_DEBUG_EXIT();
	return result;
}

std::vector<uint8_t>
a65_assembler::evaluate_pragma(
	__in a65_parser &parser,
	__in a65_tree &tree
	)
{
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Parser=%p, Tree=%p", &parser, &tree);

	// TODO: evaluate pragma

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
		A65_THROW_EXCEPTION_INFO("Define not found", "[%u]%s", name.size(), A65_STRING_CHECK(name));
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
		A65_THROW_EXCEPTION_INFO("Label not found", "[%u]%s", name.size(), A65_STRING_CHECK(name));
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

std::vector<uint8_t>
a65_assembler::form_command(
	__in int type,
	__in int mode,
	__in_opt uint16_t immediate
	)
{
	size_t length;
	std::vector<uint8_t> result;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Mode=%u(%s), Immediate=%u(%04x)", type, A65_TOKEN_COMMAND_STRING(type),
		mode, A65_TOKEN_COMMAND_MODE_STRING(mode), immediate, immediate);

	if(!is_valid_command(type, mode)) {
		A65_THROW_EXCEPTION_INFO("Unsupported command type/mode", "%s %s, %u(%04x)", A65_TOKEN_COMMAND_STRING(type),
			A65_TOKEN_COMMAND_MODE_STRING(mode), immediate, immediate);
	}

	result.push_back(A65_ASSEMBLER_COMMAND_OPCODE(type, mode));

	length = A65_ASSEMBLER_COMMAND_LENGTH(type, mode);
	switch(length) {
		case A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE:
			break;
		case A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE:
			result.push_back(immediate & UINT8_MAX);
			break;
		case A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD:
			result.push_back(immediate & UINT8_MAX);
			result.push_back((immediate >> CHAR_BIT) & UINT8_MAX);
			break;
		default:
			A65_THROW_EXCEPTION_INFO("Unsupported command length", "%u", length);
			break;
	}

	A65_DEBUG_EXIT();
	return result;
}

bool
a65_assembler::is_valid_command(
	__in int type,
	__in int mode
	) const
{
	bool result;
	std::map<int, std::map<int, std::pair<uint8_t, size_t>>>::const_iterator entry;

	A65_DEBUG_ENTRY_INFO("Type=%u(%s), Mode=%u(%s)", type, A65_TOKEN_COMMAND_STRING(type), mode, A65_TOKEN_COMMAND_MODE_STRING(mode));

	entry = A65_ASSEMBLER_COMMAND_MAP.find(mode);

	result = (entry != A65_ASSEMBLER_COMMAND_MAP.end());
	if(result) {
		result = (entry->second.find(type) != entry->second.end());
	}

	A65_DEBUG_EXIT_INFO("Result=%x", result);
	return result;
}

void
a65_assembler::output_object(
	__in const std::string &name,
	__in_opt bool verbose
	)
{
	a65_object object;
	std::stringstream path;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Verbose=%x", name.size(), A65_STRING_CHECK(name), verbose);

	path << m_output;

	if(path.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		path << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		path << A65_ASSEMBLER_OUTPUT_OBJECT_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		path << name;
	}

	path << A65_ASSEMBLER_OUTPUT_OBJECT_EXTENSION;

	if(verbose) {
		std::cout << std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl << A65_ASSEMBLER_SECTION_OBJECT
			<< std::endl << A65_ASSEMBLER_DIVIDER;
	}

	object.import(m_section);

	if(verbose) {
		size_t size = object.size();

		std::cout << std::endl << "Path: " << path.str()
			<< std::endl << "Size: " << A65_FLOAT_PREC(2, size / A65_SECTION_KB_LENGTH) << " KB (" << size << " bytes)";

		if(!object.empty()) {
			std::cout << std::endl << object.to_string();
		}
	}

	object.write(path.str());

	if(verbose) {
		std::cout << std::endl << A65_ASSEMBLER_SECTION_DONE << std::endl;
	}

	A65_DEBUG_EXIT();
}

void
a65_assembler::output_source(
	__in const std::string &name,
	__in const std::string &source,
	__in_opt bool verbose
	)
{
	std::stringstream path;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s, Source[%u]=%p Verbose=%x", name.size(), A65_STRING_CHECK(name), source.size(), &source, verbose);

	path << m_output;

	if(path.str().back() != A65_ASSEMBLER_OUTPUT_SEPERATOR) {
		path << A65_ASSEMBLER_OUTPUT_SEPERATOR;
	}

	if(name.empty()) {
		path << A65_ASSEMBLER_OUTPUT_SOURCE_NAME_DEFAULT << "_" << A65_STRING_HEX(int, std::rand());
	} else {
		path << name;
	}

	path << A65_ASSEMBLER_OUTPUT_SOURCE_EXTENSION;

	if(verbose) {
		size_t size = source.size();

		std::cout << std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl << A65_ASSEMBLER_SECTION_SOURCE
			<< std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl << "Path: " << path.str()
			<< std::endl << "Size: " << A65_FLOAT_PREC(2, size / A65_SECTION_KB_LENGTH) << " KB (" << size << " bytes)";
	}

	a65_utility::write_file(path.str(), source);

	if(verbose) {
		std::cout << std::endl << A65_ASSEMBLER_SECTION_DONE << std::endl;
	}

	A65_DEBUG_EXIT();
}

std::string
a65_assembler::preprocess(
	__in_opt const std::string &input,
	__in_opt bool verbose
	)
{
	std::string name;
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Verbose=%x", input.size(), &input, verbose);

	if(verbose) {
		std::cout << std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl << A65_ASSEMBLER_SECTION_PREPROCESS
			<< std::endl << A65_ASSEMBLER_DIVIDER;
	}

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
			result << std::endl << preprocess(*this, tree);
		}

		a65_parser::move_next();
	}

	if(verbose) {
		// TODO: write information/statistics

		std::cout << std::endl << A65_ASSEMBLER_SECTION_DONE << std::endl;
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
			result << A65_PREPROCESSOR_INDENT << preprocess_command(parser, tree);
			break;
		case A65_TOKEN_CONSTANT:
			result << A65_TOKEN_CONSTANT_STRING(entry.subtype());
			break;
		case A65_TOKEN_DIRECTIVE:
			result << A65_PREPROCESSOR_INDENT << preprocess_directive(parser, tree);
			break;
		case A65_TOKEN_IDENTIFIER:
			result << entry.literal();
			break;
		case A65_TOKEN_LABEL:
			result << entry.literal() << A65_CHARACTER_LABEL;
			break;
		case A65_TOKEN_LITERAL:
			result << A65_CHARACTER_LITERAL << entry.literal_formatted() << A65_CHARACTER_LITERAL;
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
			result << A65_PREPROCESSOR_INDENT << preprocess_pragma(parser, tree);
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
				path << m_input << "/" << entry.literal();
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
			path << m_input << "/" << entry.literal();
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
	__in const std::string &name
	)
{
	std::map<std::string, uint16_t>::iterator entry;

	A65_DEBUG_ENTRY_INFO("Name[%u]=%s", name.size(), A65_STRING_CHECK(name));

	entry = m_define.find(name);
	if(entry == m_define.end()) {
		A65_THROW_EXCEPTION_INFO("Define not found", "[%u]%s", name.size(), A65_STRING_CHECK(name));
	}

	m_define.erase(entry);

	A65_DEBUG_EXIT();
}

void
a65_assembler::run(
	__in const std::string &input,
	__in_opt const std::string &output,
	__in_opt bool source,
	__in_opt bool verbose
	)
{
	std::string name;
	std::stringstream processed;

	A65_DEBUG_ENTRY_INFO("Input[%u]=%p, Output[%u]=%p, Source=%x, Verbose=%x", input.size(), &input, output.size(), &output, source, verbose);

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

	if(verbose) {
		std::cout << A65 << " Assembler " << A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
			<< std::endl << A65_NOTICE
			<< std::endl
			<< std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl << "Input: " << input << ", Output: " << m_output
			<< std::endl << A65_ASSEMBLER_DIVIDER
			<< std::endl;
	}

	processed << A65_CHARACTER_COMMENT << " " << A65 << " Preprocessor "
			<< A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_NOTICE
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_ASSEMBLER_DIVIDER
		<< std::endl << A65_CHARACTER_COMMENT << " Input: " << input << ", Output: " << m_output
		<< std::endl << A65_CHARACTER_COMMENT << " " << A65_ASSEMBLER_DIVIDER << std::endl
		<< std::endl << A65_PREPROCESSOR_INDENT << A65_TOKEN_PRAGMA_STRING(A65_TOKEN_PRAGMA_METADATA)
			<< " " << A65_CHARACTER_LITERAL << a65_stream::path() << A65_CHARACTER_LITERAL
			<< " " << a65_stream::line() << preprocess(std::string(), verbose);

	if(source) {
		output_source(name, processed.str(), verbose);
	}

	a65_parser::load(processed.str(), false);
	a65_assembler::clear();
	evaluate(processed.str(), verbose);
	output_object(name, verbose);

	if(verbose) {
		std::cout << std::endl << A65_ASSEMBLER_EXIT << std::endl;
	}

	A65_DEBUG_EXIT();
}
