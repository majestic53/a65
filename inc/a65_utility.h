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

#ifndef A65_UTILITY_H_
#define A65_UTILITY_H_

#include "./a65_utility_type.h"

class a65_utility {

	public:

		static std::string data_as_string(
			__in const std::vector<uint8_t> &data,
			__in_opt uint16_t origin = 0
			);

		static void debug_print(
			__in int level,
			__in const std::string &prefix,
			__in const std::string &message,
			__in const std::string &file,
			__in const std::string &function,
			__in size_t line,
			__in const char *format,
			...
			);

		static std::string file_prefix(
			__in const std::string &path
			);

		static size_t read_file(
			__in const std::string &path,
			__inout std::string &data
			);

		static size_t read_file(
			__in const std::string &path,
			__inout std::vector<uint8_t> &data
			);

		static void throw_exception(
			__in const std::string &message,
			__in const std::string &file,
			__in const std::string &function,
			__in size_t line,
			__in const char *format,
			...
			);

		static void write_file(
			__in const std::string &path,
			__in const std::string &data
			);

		static void write_file(
			__in const std::string &path,
			__in const std::vector<uint8_t> &data
			);
};

#endif // A65_UTILITY_H_
