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

static std::string g_output;

int
a65_archive(
	__in int count,
	__in const char **input,
	__in const char *output,
	__in const char *name,
	__in int verbose
	)
{
	int result = EXIT_SUCCESS;

	A65_DEBUG_ENTRY_INFO("Input[%i]=%p, Output=%p, Name=%p, Verbose=%x", count, input, output, name, verbose);

	try {
		a65_assembler assembler;

		if(!name) {
			A65_THROW_EXCEPTION_INFO("Invalid name", "%p", name);
		}

		if(!output) {
			A65_THROW_EXCEPTION_INFO("Invalid output path", "%p", output);
		}

		g_output = assembler.archive(std::vector<std::string>(input, input + count), output, name, verbose);
	} catch(std::exception &exc) {
		g_error = exc.what();
		result = EXIT_FAILURE;
	}

	A65_DEBUG_EXIT_INFO("Result=%i(%x)", result, result);
	return result;
}

int
a65_assemble(
	__in const char *input,
	__in const char *output,
	__in int source,
	__in int verbose
	)
{
	int result = EXIT_SUCCESS;

	A65_DEBUG_ENTRY_INFO("Input=%p, Output=%p, Source=%x, Verbose=%x", input, output, source, verbose);

	try {
		a65_assembler assembler;

		if(!input) {
			A65_THROW_EXCEPTION_INFO("Invalid input path", "%p", input);
		}

		if(!output) {
			A65_THROW_EXCEPTION_INFO("Invalid output path", "%p", output);
		}

		g_output = assembler.assemble(input, output, source, verbose);
	} catch(std::exception &exc) {
		g_error = exc.what();
		result = EXIT_FAILURE;
	}

	A65_DEBUG_EXIT_INFO("Result=%i(%x)", result, result);
	return result;
}

const char *
a65_error(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", g_error.size(), A65_STRING_CHECK(g_error));
	return g_error.c_str();
}

int
a65_link(
	__in int count,
	__in const char **input,
	__in const char *output,
	__in const char *name,
	__in int verbose
	)
{
	int result = EXIT_SUCCESS;

	A65_DEBUG_ENTRY_INFO("Input[%i]=%p, Output=%p, Name=%p, Verbose=%x", count, input, output, name, verbose);

	try {
		a65_assembler assembler;

		if(!name) {
			A65_THROW_EXCEPTION_INFO("Invalid name path", "%p", name);
		}

		if(!output) {
			A65_THROW_EXCEPTION_INFO("Invalid output path", "%p", output);
		}

		g_output = assembler.link(std::vector<std::string>(input, input + count), output, name, verbose);
	} catch(std::exception &exc) {
		g_error = exc.what();
		result = EXIT_FAILURE;
	}

	A65_DEBUG_EXIT_INFO("Result=%i(%x)", result, result);
	return result;
}

const char *
a65_output_path(void)
{
	A65_DEBUG_ENTRY();
	A65_DEBUG_EXIT_INFO("Result[%u]=%s", g_output.size(), A65_STRING_CHECK(g_output));
	return g_output.c_str();
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
