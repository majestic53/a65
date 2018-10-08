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

#ifndef A65_TYPE_H_
#define A65_TYPE_H_

#include "../inc/a65.h"
#include "../inc/a65_utility.h"

#define A65_ARGUMENTS_MIN 2

enum {
	A65_FLAG_HELP = 0,
	A65_FLAG_OUTPUT,
	A65_FLAG_SOURCE,
	A65_FLAG_VERBOSE,
	A65_FLAG_VERSION,
};

#define A65_FLAG_DELIMITER "-"

#define A65_FLAG_MAX A65_FLAG_VERSION

static const std::string A65_FLAG_LONG_STR[] = {
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "help",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "output",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "source",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "verbose",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "version",
	};

#define A65_FLAG_LONG_STRING(_TYPE_) \
	(((_TYPE_) > A65_FLAG_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_FLAG_LONG_STR[_TYPE_]))

static const std::string A65_FLAG_SHORT_STR[] = {
	A65_FLAG_DELIMITER "h",
	A65_FLAG_DELIMITER "o",
	A65_FLAG_DELIMITER "s",
	A65_FLAG_DELIMITER "a",
	A65_FLAG_DELIMITER "v",
	};

#define A65_FLAG_SHORT_STRING(_TYPE_) \
	(((_TYPE_) > A65_FLAG_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_FLAG_SHORT_STR[_TYPE_]))

static const std::map<std::string, int> A65_FLAG_MAP = {
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_HELP), A65_FLAG_HELP),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_OUTPUT), A65_FLAG_OUTPUT),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_SOURCE), A65_FLAG_SOURCE),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_VERBOSE), A65_FLAG_VERBOSE),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_VERSION), A65_FLAG_VERSION),

	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_HELP), A65_FLAG_HELP),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_OUTPUT), A65_FLAG_OUTPUT),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_SOURCE), A65_FLAG_SOURCE),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_VERBOSE), A65_FLAG_VERBOSE),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_VERSION), A65_FLAG_VERSION),
	};

#define A65_IS_FLAG(_STRING_) \
	(A65_FLAG_MAP.find(_STRING_) != A65_FLAG_MAP.end())

#define A65_FLAG_ID(_STRING_) \
	A65_FLAG_MAP.find(_STRING_)->second

enum {
	A65_FLAG_REQUIREMENT_COUNT = 0,
	A65_FLAG_REQUIREMENT_FORMAT,
	A65_FLAG_REQUIREMENT_REQUIRED,
};

static const std::map<int, std::tuple<size_t, std::string, bool>> A65_FLAG_REQUIREMENT_MAP = {
	std::make_pair(A65_FLAG_HELP, std::make_tuple(0, std::string(), false)),
	std::make_pair(A65_FLAG_OUTPUT, std::make_tuple(1, "<path>", false)),
	std::make_pair(A65_FLAG_SOURCE, std::make_tuple(0, std::string(), false)),
	std::make_pair(A65_FLAG_VERBOSE, std::make_tuple(0, std::string(), false)),
	std::make_pair(A65_FLAG_VERSION, std::make_tuple(0, std::string(), false)),
	};

#define A65_FLAG_PARAMETER_COUNT(_TYPE_) \
	std::get<A65_FLAG_REQUIREMENT_COUNT>(A65_FLAG_REQUIREMENT_MAP.find(_TYPE_)->second)

#define A65_FLAG_PARAMETER_FORMAT(_TYPE_) \
	std::get<A65_FLAG_REQUIREMENT_FORMAT>(A65_FLAG_REQUIREMENT_MAP.find(_TYPE_)->second)

#define A65_IS_FLAG_REQUIRED(_TYPE_) \
	std::get<A65_FLAG_REQUIREMENT_REQUIRED>(A65_FLAG_REQUIREMENT_MAP.find(_TYPE_)->second)

#endif // A65_TYPE_H_
