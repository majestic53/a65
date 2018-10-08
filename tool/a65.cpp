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

#include <iostream>
#include "./a65_type.h"

void
display_usage(
	__in_opt bool verbose = false
	)
{
	// TODO: display usage
}

void
display_version(
	__in_opt bool verbose = false
	)
{
	// TODO: display version
}

int
parse(
	__in const std::vector<std::string> &arguments,
	__inout std::string &input,
	__inout std::string &output,
	__inout bool &help,
	__inout bool &version,
	__inout bool &source,
	__inout bool &verbose
	)
{
	int result;

	// TODO: parse user input
	result = EXIT_SUCCESS;
	// ---

	return result;
}

int
run(
	__in const std::string &input,
	__in const std::string &output,
	__in bool source,
	__in bool verbose
	)
{
	int result;

	// TODO: run with user input
	result = a65_assemble("./doc/example.asm", "./bin");
	if(result) {
		std::cerr << "A65: " << a65_error() << std::endl;
		result = EXIT_FAILURE;
	}
	// ---

	return result;
}

int
main(
	__in int argc,
	__in const char *argv[]
	)
{
	int result = EXIT_SUCCESS;

	if(argc >= A65_ARGUMENTS_MIN) {
		std::string input, output;
		bool help = false, source = false, verbose = false, version = false;

		result = parse(std::vector<std::string>(argv + 1, argv + argc), input, output, help, version, source, verbose);
		if(result == EXIT_SUCCESS) {

			if(help) {
				display_usage(true);
			} else if(version) {
				display_version();
			} else {
				result = run(input, output, source, verbose);
			}
		}
	} else {
		display_usage();
		result = EXIT_FAILURE;
	}

	return result;
}
