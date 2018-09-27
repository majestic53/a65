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

#include "../inc/a65.h"
#include "../inc/a65_assembler.h"
#include "../inc/a65_utility.h"

static std::string g_error;

int
a65_assemble(
	__in const char *input,
	__in const char *output
	)
{
	int result = EXIT_SUCCESS;

	A65_DEBUG_ENTRY_INFO("Input=%p, Output=%p", input, output);

	try {

		if(!input) {
			A65_THROW_EXCEPTION_INFO("Invalid input path", "%p", input);
		}

		if(!output) {
			A65_THROW_EXCEPTION_INFO("Invalid output path", "%p", input);
		}

		// TODO: debugging
		a65_parser parser(input);

		while(parser.has_next()) {
			std::cout << parser.to_string() << std::endl;
			parser.move_next();
		}

		std::cout << parser.to_string() << std::endl;
		// ---

	} catch(std::exception &exc) {
		g_error = exc.what();
		result = EXIT_FAILURE;
	}

	return result;
}

const char *
a65_error(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", g_error.size(), A65_STRING_CHECK(g_error));
	return g_error.c_str();
}

void
a65_version(
	__inout_opt int *major,
	__inout_opt int *minor,
	__inout_opt int *revision
	)
{
	A65_DEBUG_ENTRY_INFO("Major=%p, Minor=%p, Revision=%p", major, minor, revision);

	if(major) {
		*major = A65_VERSION_MAJOR;
	}

	if(minor) {
		*minor = A65_VERSION_MINOR;
	}

	if(revision) {
		*revision = A65_VERSION_REVISION;
	}

	A65_DEBUG_EXIT();
}
