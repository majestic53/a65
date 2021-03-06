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

#define A65_ARGUMENTS_HEAD "a65"
#define A65_ARGUMENTS_TAIL "input..."

#define A65_ARGUMENTS_MIN 2

#define A65_EXTENSION "."
#define A65_EXTENSION_ARCHIVE "a"
#define A65_EXTENSION_OBJECT "o"

#define A65_FLAG(_TYPE_) \
	(1 << (_TYPE_))

#define A65_FLAG_APPEND(_TYPE_, _FLAG_) \
	((_FLAG_) |= A65_FLAG(_TYPE_))

#define A65_FLAG_CONTAINS(_TYPE_, _FLAG_) \
	(((_FLAG_) & A65_FLAG(_TYPE_)) == A65_FLAG(_TYPE_))

enum {
	A65_FLAG_ARCHIVE = 0,
	A65_FLAG_COMPILE,
	A65_FLAG_EXPORT,
	A65_FLAG_HELP,
	A65_FLAG_IHEX,
	A65_FLAG_NO_BINARY,
	A65_FLAG_OUTPUT,
	A65_FLAG_SOURCE,
	A65_FLAG_VERBOSE,
	A65_FLAG_VERSION,
};

#define A65_FLAG_COLUMN_WIDTH 14

#define A65_FLAG_DELIMITER "-"

#define A65_FLAG_MAX A65_FLAG_VERSION

static const std::string A65_FLAG_DESC_STR[] = {
	"Output archive file",
	"Output binary file",
	"Output export file",
	"Display help information",
	"Output ihex file",
	"Exclude binary file output",
	"Specify output directory",
	"Enable source output",
	"Enable verbose output",
	"Display version information",
	};

#define A65_FLAG_DESCRIPTION_STRING(_TYPE_) \
	(((_TYPE_) > A65_FLAG_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_FLAG_DESC_STR[_TYPE_]))

static const std::string A65_FLAG_LONG_STR[] = {
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "archive",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "compile",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "export",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "help",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "ihex",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "no-binary",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "output",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "source",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "verbose",
	A65_FLAG_DELIMITER A65_FLAG_DELIMITER "version",
	};

#define A65_FLAG_LONG_STRING(_TYPE_) \
	(((_TYPE_) > A65_FLAG_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_FLAG_LONG_STR[_TYPE_]))

static const std::string A65_FLAG_SHORT_STR[] = {
	A65_FLAG_DELIMITER "a",
	A65_FLAG_DELIMITER "c",
	A65_FLAG_DELIMITER "e",
	A65_FLAG_DELIMITER "h",
	A65_FLAG_DELIMITER "i",
	A65_FLAG_DELIMITER "n",
	A65_FLAG_DELIMITER "o",
	A65_FLAG_DELIMITER "s",
	A65_FLAG_DELIMITER "b",
	A65_FLAG_DELIMITER "v",
	};

#define A65_FLAG_SHORT_STRING(_TYPE_) \
	(((_TYPE_) > A65_FLAG_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_FLAG_SHORT_STR[_TYPE_]))

static const std::map<std::string, int> A65_FLAG_MAP = {
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_ARCHIVE), A65_FLAG_ARCHIVE),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_COMPILE), A65_FLAG_COMPILE),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_EXPORT), A65_FLAG_EXPORT),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_HELP), A65_FLAG_HELP),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_IHEX), A65_FLAG_IHEX),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_NO_BINARY), A65_FLAG_NO_BINARY),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_OUTPUT), A65_FLAG_OUTPUT),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_SOURCE), A65_FLAG_SOURCE),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_VERBOSE), A65_FLAG_VERBOSE),
	std::make_pair(A65_FLAG_LONG_STRING(A65_FLAG_VERSION), A65_FLAG_VERSION),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_ARCHIVE), A65_FLAG_ARCHIVE),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_COMPILE), A65_FLAG_COMPILE),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_EXPORT), A65_FLAG_EXPORT),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_HELP), A65_FLAG_HELP),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_IHEX), A65_FLAG_IHEX),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_NO_BINARY), A65_FLAG_NO_BINARY),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_OUTPUT), A65_FLAG_OUTPUT),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_SOURCE), A65_FLAG_SOURCE),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_VERBOSE), A65_FLAG_VERBOSE),
	std::make_pair(A65_FLAG_SHORT_STRING(A65_FLAG_VERSION), A65_FLAG_VERSION),
	};

#define A65_IS_FLAG(_STRING_) \
	(A65_FLAG_MAP.find(_STRING_) != A65_FLAG_MAP.end())

#define A65_FLAG_ID(_STRING_) \
	A65_FLAG_MAP.find(_STRING_)->second

static const std::vector<std::string> A65_FLAG_FORMAT_EMPTY;

static const std::vector<std::string> A65_FLAG_FORMAT_NAME = { "name" };

static const std::vector<std::string> A65_FLAG_FORMAT_OUTPUT = { "output" };

static const std::map<int, std::pair<std::vector<std::string>, bool>> A65_FLAG_REQUIREMENT_MAP = {
	std::make_pair(A65_FLAG_ARCHIVE, std::make_pair(A65_FLAG_FORMAT_NAME, false)),
	std::make_pair(A65_FLAG_COMPILE, std::make_pair(A65_FLAG_FORMAT_NAME, false)),
	std::make_pair(A65_FLAG_EXPORT, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	std::make_pair(A65_FLAG_HELP, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	std::make_pair(A65_FLAG_IHEX, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	std::make_pair(A65_FLAG_NO_BINARY, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	std::make_pair(A65_FLAG_OUTPUT, std::make_pair(A65_FLAG_FORMAT_OUTPUT, false)),
	std::make_pair(A65_FLAG_SOURCE, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	std::make_pair(A65_FLAG_VERBOSE, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	std::make_pair(A65_FLAG_VERSION, std::make_pair(A65_FLAG_FORMAT_EMPTY, false)),
	};

#define A65_FLAG_FORMAT(_TYPE_) \
	A65_FLAG_REQUIREMENT_MAP.find(_TYPE_)->second.first

#define A65_IS_FLAG_REQUIRED(_TYPE_) \
	A65_FLAG_REQUIREMENT_MAP.find(_TYPE_)->second.second

#define A65_VERBOSE_DIVIDER "============================================"

#define A65_VERBOSE_SECTION_ARCHIVE "Generating archive file"
#define A65_VERBOSE_SECTION_COMPILE "Generating binary file"
#define A65_VERBOSE_SECTION_OBJECT "Generating object file(s)"

#endif // A65_TYPE_H_
