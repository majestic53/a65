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
assemble(
	__inout std::vector<std::string> &objects,
	__in const std::vector<std::string> &sources,
	__in const std::string &output,
	__in bool source,
	__in bool verbose
	)
{
	int result = EXIT_SUCCESS;
	std::vector<std::string>::const_iterator entry;

	if(verbose) {
		std::cout << A65 << " Assembler " << A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
			<< std::endl << A65_NOTICE
			<< std::endl;
	}

	for(entry = sources.begin(); entry != sources.end(); ++entry) {
		result = a65_assemble(entry->c_str(), output.c_str(), source, verbose);

		if(result) {
			std::cerr << A65 << ": " << a65_error() << std::endl;
			result = EXIT_FAILURE;
			break;
		}

		objects.push_back(a65_output_path());
	}

	return result;
}

int
archive(
	__inout std::vector<std::string> &objects,
	__in const std::vector<std::string> &sources,
	__in const std::string &output,
	__in const std::string &name,
	__in bool source,
	__in bool verbose
	)
{
	int result = EXIT_SUCCESS;

	if(!sources.empty()) {
		result = assemble(objects, sources, output, source, verbose);
	} else if(verbose) {
		std::cout << A65 << " Assembler " << A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
			<< std::endl << A65_NOTICE
			<< std::endl;
	}

	if(result == EXIT_SUCCESS) {
		std::vector<const char *> inputs;

		for(std::vector<std::string>::iterator entry = objects.begin(); entry != objects.end(); ++entry) {
			inputs.push_back(entry->c_str());
		}

		result = a65_archive(inputs.size(), (const char **)&inputs[0], output.c_str(), name.c_str(), verbose);
		if(result) {
			std::cerr << A65 << ": " << a65_error() << std::endl;
			result = EXIT_FAILURE;
		}
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
link(
	__inout std::vector<std::string> &objects,
	__in const std::vector<std::string> &archives,
	__in const std::vector<std::string> &sources,
	__in const std::string &output,
	__in const std::string &name,
	__in bool source,
	__in bool verbose
	)
{
	int result = EXIT_SUCCESS;

	if(!sources.empty()) {
		result = assemble(objects, sources, output, source, verbose);
	} else if(verbose) {
		std::cout << A65 << " Assembler " << A65_VERSION_MAJOR << "." << A65_VERSION_MINOR << "." << A65_VERSION_REVISION
			<< std::endl << A65_NOTICE
			<< std::endl;
	}

	if(result == EXIT_SUCCESS) {
		std::vector<const char *> inputs;

		for(std::vector<std::string>::iterator entry = objects.begin(); entry != objects.end(); ++entry) {
			inputs.push_back(entry->c_str());
		}

		for(std::vector<std::string>::const_iterator entry = archives.begin(); entry != archives.end(); ++entry) {
			inputs.push_back(entry->c_str());
		}

		result = a65_link(inputs.size(), (const char **)&inputs[0], output.c_str(), name.c_str(), verbose);
		if(result) {
			std::cerr << A65 << ": " << a65_error() << std::endl;
			result = EXIT_FAILURE;
		}
	}

	return result;
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
					case A65_FLAG_HELP:
						A65_FLAG_APPEND(A65_FLAG_HELP, flags);
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
						A65_FLAG_APPEND(A65_FLAG_SOURCE, flags);
						break;
					case A65_FLAG_VERBOSE:
						A65_FLAG_APPEND(A65_FLAG_VERBOSE, flags);
						break;
					case A65_FLAG_VERSION:
						A65_FLAG_APPEND(A65_FLAG_VERSION, flags);
						break;
					default:
						stream << "Invalid flag: " << *argument;
						result = EXIT_FAILURE;
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
				std::vector<std::string> archives, objects, sources;
				bool source = A65_FLAG_CONTAINS(A65_FLAG_SOURCE, flags),
					verbose = A65_FLAG_CONTAINS(A65_FLAG_VERBOSE, flags);

				for(std::vector<std::string>::iterator entry = input.begin(); entry != input.end(); ++entry) {
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

				if(A65_FLAG_CONTAINS(A65_FLAG_ARCHIVE, flags)) {
					result = archive(objects, sources, output, name, source, verbose);
				} else if(A65_FLAG_CONTAINS(A65_FLAG_COMPILE, flags)) {
					result = link(objects, archives, sources, output, name, source, verbose);
				} else {
					std::vector<std::string> objects;

					result = assemble(objects, sources, output, source, verbose);
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
