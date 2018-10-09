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
	std::cout << A65_ARGUMENTS_HEAD;

	for(int flag = 0; flag <= A65_FLAG_MAX; ++flag) {
		std::vector<std::string>::const_iterator parameter;

		if(!flag) {
			std::cout << " ";
		}

		std::cout << "[" << A65_FLAG_SHORT_STRING(flag);

		const std::vector<std::string> &parameters = A65_FLAG_FORMAT(flag);
		for(parameter = parameters.begin(); parameter != parameters.end(); ++parameter) {
			std::cout << " " << *parameter;
		}

		std::cout << "]";
	}

	std::cout << " " << A65_ARGUMENTS_TAIL;

	if(verbose) {
		std::cout << std::endl;

		for(int flag = 0; flag <= A65_FLAG_MAX; ++flag) {
			std::cout << std::endl << A65_FLAG_SHORT_STRING(flag) << "|" << A65_COLUMN_WIDTH(A65_FLAG_COLUMN_WIDTH)
				<< A65_FLAG_LONG_STRING(flag) << A65_FLAG_DESCRIPTION_STRING(flag);
		}
	}

	std::cout << std::endl;
}

void
display_version(
	__in_opt bool verbose = false
	)
{
	if(verbose) {
		std::cout << A65 << " " << A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
			<< std::endl << A65_NOTICE;
	} else {
		std::cout << A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION;
	}

	std::cout << std::endl;
}

int
parse(
	__in const std::vector<std::string> &arguments,
	__inout std::string &error,
	__inout std::string &input,
	__inout std::string &output,
	__inout bool &help,
	__inout bool &version,
	__inout bool &source,
	__inout bool &verbose
	)
{
	std::stringstream stream;
	int result = EXIT_SUCCESS;
	std::vector<std::string>::const_iterator argument;

	help = false;
	source = false;
	verbose = false;
	version = false;

	error.clear();
	input.clear();
	output.clear();

	for(argument = arguments.begin(); argument != arguments.end(); ++argument) {

		if((*argument).front() == std::string(A65_FLAG_DELIMITER).front()) {

			if(A65_IS_FLAG(*argument)) {
				int id = A65_FLAG_ID(*argument);

				switch(id) {
					case A65_FLAG_HELP:
						help = true;
						break;
					case A65_FLAG_OUTPUT:

						if(argument == (arguments.end() - 1)) {
							stream << "Undefined flag parameter: " << *argument;
							result = EXIT_FAILURE;
						} else {
							output = *(++argument);
						}
						break;
					case A65_FLAG_SOURCE:
						source = true;
						break;
					case A65_FLAG_VERBOSE:
						verbose = true;
						break;
					case A65_FLAG_VERSION:
						version = true;
						break;
					default:
						stream << "Invalid flag: " << *argument;
						result = EXIT_FAILURE;
				}
			} else {
				stream << "Undefined flag: " << *argument;
				result = EXIT_FAILURE;
			}
		} else if(input.empty()) {
			input = *argument;
		} else {
			stream << "Input redefined: " << *argument;
			result = EXIT_FAILURE;
			break;
		}

		if((result != EXIT_SUCCESS) || help || version) {
			break;
		}
	}

	if((result == EXIT_SUCCESS) && input.empty() && !help && !version) {
		stream << "Input undefined";
		result = EXIT_FAILURE;
	}

	if(result != EXIT_SUCCESS) {
		error = stream.str();
	}

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
		std::string error, input, output;
		bool help = false, source = false, verbose = false, version = false;

		result = parse(std::vector<std::string>(argv + 1, argv + argc), error, input, output, help, version, source, verbose);
		if(result == EXIT_SUCCESS) {

			if(help) {
				display_usage(true);
			} else if(version) {
				display_version();
			} else {

				result = a65_assemble(input.c_str(), output.c_str(), source, verbose);
				if(result) {
					std::cerr << A65 << ": " << a65_error() << std::endl;
					result = EXIT_FAILURE;
				}
			}
		} else {
			display_usage();

			if(!error.empty()) {
				std::cerr << std::endl << error << std::endl;
			}
		}
	} else {
		display_usage();
		result = EXIT_FAILURE;
	}

	return result;
}
