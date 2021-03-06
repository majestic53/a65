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

int
build_objects(
	__inout std::vector<std::string> &objects,
	__in const std::vector<std::string> &sources,
	__in const std::string &output,
	__in bool header,
	__in bool source
	)
{
	int result = EXIT_SUCCESS;
	std::vector<std::string>::const_iterator entry;

	for(entry = sources.begin(); entry != sources.end(); ++entry) {

		result = a65_build_object(entry->c_str(), output.c_str(), header, source);
		if(result) {
			break;
		}

		objects.push_back(a65_output_path());
	}

	return result;
}

int
build_archive(
	__inout std::vector<std::string> &objects,
	__in const std::vector<std::string> &sources,
	__in const std::string &output,
	__in const std::string &name,
	__in bool header,
	__in bool source
	)
{
	int result = EXIT_SUCCESS;

	if(!sources.empty()) {
		result = build_objects(objects, sources, output, header, source);
	}

	if(result == EXIT_SUCCESS) {
		std::vector<const char *> inputs;

		for(std::vector<std::string>::iterator entry = objects.begin(); entry != objects.end(); ++entry) {
			inputs.push_back(entry->c_str());
		}

		result = a65_build_archive(inputs.size(), (const char **)&inputs[0], output.c_str(), name.c_str());
	}

	return result;
}

int
compile(
	__inout std::vector<std::string> &objects,
	__in const std::vector<std::string> &archives,
	__in const std::vector<std::string> &sources,
	__in const std::string &output,
	__in const std::string &name,
	__in bool header,
	__in bool source,
	__in bool binary,
	__in bool ihex
	)
{
	int result = EXIT_SUCCESS;

	if(!sources.empty()) {
		result = build_objects(objects, sources, output, header, source);
	}

	if(result == EXIT_SUCCESS) {
		std::vector<const char *> inputs;

		for(std::vector<std::string>::iterator entry = objects.begin(); entry != objects.end(); ++entry) {
			inputs.push_back(entry->c_str());
		}

		for(std::vector<std::string>::const_iterator entry = archives.begin(); entry != archives.end(); ++entry) {
			inputs.push_back(entry->c_str());
		}

		result = a65_compile(inputs.size(), (const char **)&inputs[0], output.c_str(), name.c_str(), binary, ihex);
	}

	return result;
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

void
display_usage(
	__in_opt bool verbose = false
	)
{

	if(verbose) {
		display_version(true);
		std::cout << std::endl;
	}

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

int
parse(
	__in const std::vector<std::string> &arguments,
	__inout std::vector<std::string> &input,
	__inout std::string &output,
	__inout std::string &name,
	__inout std::string &error,
	__inout int &flags
	)
{
	std::stringstream stream;
	int result = EXIT_SUCCESS;
	std::vector<std::string>::const_iterator argument;

	flags = 0;

	error.clear();
	input.clear();
	name.clear();
	output.clear();

	for(argument = arguments.begin(); argument != arguments.end(); ++argument) {

		if((*argument).front() == std::string(A65_FLAG_DELIMITER).front()) {

			if(A65_IS_FLAG(*argument)) {
				int id = A65_FLAG_ID(*argument);

				switch(id) {
					case A65_FLAG_ARCHIVE:
					case A65_FLAG_COMPILE:

						switch(id) {
							case A65_FLAG_ARCHIVE:
								A65_FLAG_APPEND(A65_FLAG_ARCHIVE, flags);
								break;
							case A65_FLAG_COMPILE:
								A65_FLAG_APPEND(A65_FLAG_COMPILE, flags);
								break;
							default:
								break;
						}

						if(argument == (arguments.end() - 1)) {
							stream << "Undefined flag parameter: " << *argument;
							result = EXIT_FAILURE;
						} else if(!name.empty()) {
							stream << "Unsupported flag combination: " << *argument;
							result = EXIT_FAILURE;
						} else {
							name = *(++argument);
						}
						break;
					case A65_FLAG_EXPORT:
					case A65_FLAG_IHEX:
					case A65_FLAG_HELP:
					case A65_FLAG_NO_BINARY:
					case A65_FLAG_SOURCE:
					case A65_FLAG_VERBOSE:
					case A65_FLAG_VERSION:
						A65_FLAG_APPEND(id, flags);
						break;
					case A65_FLAG_OUTPUT:

						if(argument == (arguments.end() - 1)) {
							stream << "Undefined flag parameter: " << *argument;
							result = EXIT_FAILURE;
						} else {
							output = *(++argument);
						}
						break;
					default:
						stream << "Invalid flag: " << *argument;
						result = EXIT_FAILURE;
						break;
				}
			} else {
				stream << "Undefined flag: " << *argument;
				result = EXIT_FAILURE;
			}
		} else {
			input.push_back(*argument);
		}

		if((result != EXIT_SUCCESS)
				|| A65_FLAG_CONTAINS(A65_FLAG_HELP, flags)
				|| A65_FLAG_CONTAINS(A65_FLAG_VERSION, flags)) {
			break;
		}
	}

	if((result == EXIT_SUCCESS)
			&& input.empty()
			&& !A65_FLAG_CONTAINS(A65_FLAG_HELP, flags)
			&& !A65_FLAG_CONTAINS(A65_FLAG_VERSION, flags)) {
		stream << "Input undefined";
		result = EXIT_FAILURE;
	}

	if(result != EXIT_SUCCESS) {
		error = stream.str();
	}

	return result;
}

int
run(
	__in const std::vector<std::string> &input,
	__in const std::string &output,
	__in const std::string &name,
	__in int flags
	)
{
	int result = EXIT_SUCCESS;
	std::vector<std::string>::const_iterator entry;
	std::vector<std::string> archives, objects, sources;
	bool binary = !A65_FLAG_CONTAINS(A65_FLAG_NO_BINARY, flags),
		header = A65_FLAG_CONTAINS(A65_FLAG_EXPORT, flags),
		ihex = A65_FLAG_CONTAINS(A65_FLAG_IHEX, flags),
		source = A65_FLAG_CONTAINS(A65_FLAG_SOURCE, flags),
		verbose = A65_FLAG_CONTAINS(A65_FLAG_VERBOSE, flags);

	if(verbose) {
		display_version(true);
		std::cout << std::endl;
	}

	if(!input.empty()) {

		if(verbose) {
			std::cout << "Inputs: ";
		}

		for(entry = input.begin(); entry != input.end(); ++entry) {

			if(verbose) {

				if(entry != input.begin()) {
					std::cout << ", ";
				}

				std::cout << *entry;
			}

			size_t dot = entry->find_last_of(A65_EXTENSION);

			if(dot != std::string::npos) {
				std::string extension = entry->substr(dot + 1);

				if(extension == A65_EXTENSION_ARCHIVE) {
					archives.push_back(*entry);
				} else if(extension == A65_EXTENSION_OBJECT) {
					objects.push_back(*entry);
				} else {
					sources.push_back(*entry);
				}
			}
		}

		if(verbose) {
			std::cout << std::endl;
		}
	}

	if(verbose) {
		std::cout << "Output: " << A65_STRING_CHECK(output) << std::endl;
	}

	if(A65_FLAG_CONTAINS(A65_FLAG_ARCHIVE, flags)) {

		if(verbose) {
			std::cout << A65_VERBOSE_DIVIDER
				<< std::endl << A65_VERBOSE_SECTION_ARCHIVE
				<< std::endl << "Name: " << name
				<< std::endl << A65_VERBOSE_DIVIDER
				<< std::endl;
		}

		result = build_archive(objects, sources, output, name, header, source);
	} else if(A65_FLAG_CONTAINS(A65_FLAG_COMPILE, flags)) {

		if(verbose) {
			std::cout << A65_VERBOSE_DIVIDER
				<< std::endl << A65_VERBOSE_SECTION_COMPILE
				<< std::endl << "Name: " << name
				<< std::endl << A65_VERBOSE_DIVIDER
				<< std::endl;
		}

		result = compile(objects, archives, sources, output, name, header, source, binary, ihex);
	} else {

		if(verbose) {
			std::cout << A65_VERBOSE_DIVIDER
				<< std::endl << A65_VERBOSE_SECTION_OBJECT
				<< std::endl << "Sources: ";

			for(entry = sources.begin(); entry != sources.end(); ++entry) {

				if(entry != sources.begin()) {
					std::cout << ", ";
				}

				std::cout << *entry;
			}

			std::cout << std::endl << A65_VERBOSE_DIVIDER
				<< std::endl;
		}

		result = build_objects(objects, sources, output, header, source);
	}

	if(result) {
		std::cerr << "Error: " << a65_error() << std::endl;
	} else if(verbose) {
		std::cout << "Success" << std::endl;
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
		int flags = 0;
		std::vector<std::string> input;
		std::string error, name, output;

		result = parse(std::vector<std::string>(argv + 1, argv + argc), input, output, name, error, flags);
		if(result == EXIT_SUCCESS) {

			if(A65_FLAG_CONTAINS(A65_FLAG_HELP, flags)) {
				display_usage(true);
			} else if(A65_FLAG_CONTAINS(A65_FLAG_VERSION, flags)) {
				display_version();
			} else {
				result = run(input, output, name, flags);
			}
		} else {
			display_usage();
		}

		if(!error.empty()) {
			std::cerr << std::endl << error << std::endl;
		}
	} else {
		display_usage();
		result = EXIT_FAILURE;
	}

	return result;
}
