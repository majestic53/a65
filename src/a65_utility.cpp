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

#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../inc/a65_utility.h"

#define A65_DEBUG_COLOR_OFF "\x1b[0m"

static const std::string A65_DEBUG_COLOR_STR[] = {
	"\x1b[91m", "\x1b[93m", "\x1b[94m", "\x1b[90m",
	};

#define A65_DEBUG_COLOR_STRING(_TYPE_) \
	(((_TYPE_) > A65_DEBUG_LEVEL_MAX) ? A65_DEBUG_COLOR_OFF : \
		A65_STRING_CHECK(A65_DEBUG_COLOR_STR[_TYPE_]))

#define A65_DEBUG_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define A65_DEBUG_TIMESTAMP_LENGTH 32

void
a65_utility::debug_print(
	__in int level,
	__in const std::string &prefix,
	__in const std::string &message,
	__in const std::string &file,
	__in const std::string &function,
	__in size_t line,
	__in const char *format,
	...
	)
{
	int length;
	std::string formatted;
	std::stringstream result;
	std::time_t timestamp = std::time(nullptr);

#ifdef TRACE_COLOR
	result << A65_DEBUG_COLOR_STRING(level);
#endif // TRACE_COLOR

	formatted.resize(A65_DEBUG_TIMESTAMP_LENGTH);
	length = std::strftime((char *)&formatted[0], formatted.size(), A65_DEBUG_TIMESTAMP_FORMAT, std::localtime(&timestamp));

	if(length > 0) {
		formatted = formatted.substr(0, length);
	} else {
		formatted = A65_STRING_MALFORMED;
	}

	result << "[" << formatted << "] {" << A65_DEBUG_LEVEL_STRING(level) << "}";
	formatted.clear();

	if(!message.empty()) {

		if(!result.str().empty()) {
			result << " ";
		}

		result << prefix << message;
	}

	if(format) {
		va_list arguments;

		va_start(arguments, format);
		length = std::vsnprintf(nullptr, 0, format, arguments);
		va_end(arguments);

		if(length > 0) {
			formatted.resize(++length);
			va_start(arguments, format);
			length = std::vsnprintf((char *)&formatted[0], formatted.size(), format, arguments);
			va_end(arguments);
		}

		if(length < 0) {
			formatted = A65_STRING_MALFORMED;
		}

		if(!formatted.empty()) {

			if(!result.str().empty()) {
				result << ": ";
			}

			result << formatted;
		}
	}

#ifndef NDEBUG
	if(!result.str().empty()) {
		result << " ";
	}

	result << "(";

	if(!file.empty()) {
		result << file << ":";
	}

	if(!function.empty()) {
		result << function << ":";
	}

	result << line << ")";
#endif // NDEBUG

#ifdef TRACE_COLOR
	result << A65_DEBUG_COLOR_OFF;
#endif // TRACE_COLOR

	switch(level) {
		case A65_DEBUG_LEVEL_ERROR:
		case A65_DEBUG_LEVEL_WARNING:
			std::cerr << result.str() << std::endl;
			break;
		default:
			std::cout << result.str() << std::endl;
			break;
	}
}

size_t
a65_utility::read_file(
	__in const std::string &path,
	__inout std::string &data
	)
{
	size_t result = 0;

	A65_DEBUG_ENTRY_INFO("Path[%u]=%s, Data=%p", path.size(), A65_STRING_CHECK(path), &data);

	if(!path.empty()) {
		int length;
		std::ifstream file;

		file = std::ifstream(path.c_str(), std::ios::binary | std::ios::in);
		if(!file) {
			A65_THROW_EXCEPTION_INFO("File not found", "[%u]%s", path.size(), A65_STRING_CHECK(path));
		}

		file.seekg(0, std::ios::end);
		length = file.tellg();
		file.seekg(0, std::ios::beg);

		if(length > 0) {
			data.resize(length);
			file.read((char *)&data[0], data.size());
		} else {
			data.clear();
		}

		file.close();

		if(length < 0) {
			A65_THROW_EXCEPTION_INFO("Malformed file", "[%u]%s", path.size(), A65_STRING_CHECK(path));
		}

		result = data.size();
	}

	A65_DEBUG_EXIT_INFO("Result[%u]=%p", result, &data);
	return result;
}

void
a65_utility::throw_exception(
	__in const std::string &message,
	__in const std::string &file,
	__in const std::string &function,
	__in size_t line,
	__in const char *format,
	...
	)
{
	std::stringstream result;

	result << message;

	if(format) {
		int length;
		va_list arguments;
		std::string formatted;

		va_start(arguments, format);
		length = std::vsnprintf(nullptr, 0, format, arguments);
		va_end(arguments);

		if(length > 0) {
			formatted.resize(++length);
			va_start(arguments, format);
			length = std::vsnprintf((char *)&formatted[0], formatted.size(), format, arguments);
			va_end(arguments);
		}

		if(length < 0) {
			formatted = A65_STRING_MALFORMED;
		}

		if(!formatted.empty()) {

			if(!result.str().empty()) {
				result << ": ";
			}

			result << formatted;
		}
	}

#ifndef NDEBUG
	if(!result.str().empty()) {
		result << " ";
	}

	result << "(";

	if(!file.empty()) {
		result << file << ":";
	}

	if(!function.empty()) {
		result << function << ":";
	}

	result << line << ")";
#endif // NDEBUG

	A65_DEBUG_MESSAGE(A65_DEBUG_LEVEL_ERROR, result.str());
	throw std::runtime_error(result.str());
}
