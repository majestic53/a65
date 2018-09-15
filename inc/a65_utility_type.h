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

#ifndef A65_UTILITY_TYPE_H_
#define A65_UTILITY_TYPE_H_

#include "./a65_define.h"

#ifndef NDEBUG
#define A65_ASSERT(_CONDITION_) { \
	if(!(_CONDITION_)) { \
		A65_THROW_EXCEPTION("Assertion failed: " A65_STRING_CONCAT(_CONDITION_)); \
	} \
	}
#else
#define A65_ASSERT(_CONDITION_)
#endif // NDEBUG

#define A65_DEBUG_ENTRY() \
	A65_DEBUG_ENTRY_INFO("", "")
#define A65_DEBUG_ENTRY_INFO(_FORMAT_, ...) \
	A65_DEBUG_PRINT(A65_DEBUG_LEVEL_VERBOSE, "+", __FUNCTION__, __FILE__, __FUNCTION__, __LINE__, \
		_FORMAT_, __VA_ARGS__)

#define A65_DEBUG_EXIT() \
	A65_DEBUG_EXIT_INFO("", "")
#define A65_DEBUG_EXIT_INFO(_FORMAT_, ...) \
	A65_DEBUG_PRINT(A65_DEBUG_LEVEL_VERBOSE, "-", __FUNCTION__, __FILE__, __FUNCTION__, __LINE__, \
		_FORMAT_, __VA_ARGS__)

#define A65_DEBUG_MESSAGE(_LEVEL_, _MESSAGE_) \
	A65_DEBUG_MESSAGE_INFO(_LEVEL_, _MESSAGE_, "", "")
#define A65_DEBUG_MESSAGE_INFO(_LEVEL_, _MESSAGE_, _FORMAT_, ...) \
	A65_DEBUG_PRINT(_LEVEL_, "", _MESSAGE_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)

#ifndef NDEBUG
#define A65_DEBUG_PRINT(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) { \
	if((_LEVEL_) <= (TRACE)) { \
		a65_utility::debug_print(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, \
			_FORMAT_, __VA_ARGS__); \
	} \
	}
#else
#define A65_DEBUG_PRINT(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...)
#endif // NDEBUG

#define A65_THROW_EXCEPTION(_MESSAGE_) \
	A65_THROW_EXCEPTION_INFO(_MESSAGE_, "", "")
#define A65_THROW_EXCEPTION_INFO(_MESSAGE_, _FORMAT_, ...) \
	a65_utility::throw_exception(_MESSAGE_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)

enum {
	A65_DEBUG_LEVEL_ERROR = 0,
	A65_DEBUG_LEVEL_WARNING,
	A65_DEBUG_LEVEL_INFORMATION,
	A65_DEBUG_LEVEL_VERBOSE,
};

#define A65_DEBUG_COLOR_OFF "\x1b[0m"

static const std::string A65_DEBUG_COLOR_STR[] = {
	"\x1b[91m", "\x1b[93m", "\x1b[94m", "\x1b[90m",
	};

#define A65_DEBUG_COLOR_STRING(_TYPE_) \
	(((_TYPE_) > A65_DEBUG_LEVEL_MAX) ? A65_DEBUG_COLOR_OFF : \
		A65_STRING_CHECK(A65_DEBUG_COLOR_STR[_TYPE_]))

#define A65_DEBUG_LEVEL_MAX A65_DEBUG_LEVEL_VERBOSE

static const std::string A65_DEBUG_LEVEL_STR[] = {
	"Error", "Warning", "Information", "Verbose",
	};

#define A65_DEBUG_LEVEL_STRING(_TYPE_) \
	(((_TYPE_) > A65_DEBUG_LEVEL_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_DEBUG_LEVEL_STR[_TYPE_]))

#define A65_DEBUG_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define A65_DEBUG_TIMESTAMP_LENGTH 32

#endif // A65_UTILITY_TYPE_H_
