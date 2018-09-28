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
	std::stringstream result;

	A65_DEBUG_ENTRY_INFO("Tree=%p", &tree);

	// TODO: parse tree and form source string

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
		std::cout << a65_parser::to_string() << std::endl;
		a65_parser::move_next();
	}

	std::cout << a65_parser::to_string() << std::endl;
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

	if(options & A65_ASSEMBLER_BINARY) {
		form_binary();
	}

	if(options & A65_ASSEMBLER_IHEX) {
		form_ihex();
	}

	if(options & A65_ASSEMBLER_LISTING) {
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
