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

#ifndef A65_TEST_TYPE_H_
#define A65_TEST_TYPE_H_

#include "../inc/a65.h"
#include "../inc/a65_utility.h"

#define A65_TEST_COLUMN_WIDTH 38

#define A65_TEST_OUTPUT "./"

#define A65_TEST_FUNCTIONAL_BINARY "binary.bin"
#define A65_TEST_FUNCTIONAL_BINARY_NAME "binary"
#define A65_TEST_FUNCTIONAL_OBJECT "object.o"
#define A65_TEST_FUNCTIONAL_OBJECT_OUTPUT "source.o"
#define A65_TEST_FUNCTIONAL_SOURCE "source.asm"

enum {
	A65_TEST_FUNCTIONAL_ADVANCED = 0,
	A65_TEST_FUNCTIONAL_BRANCH,
	A65_TEST_FUNCTIONAL_INDEX_INDIRECT,
	A65_TEST_FUNCTIONAL_INDIRECT,
	A65_TEST_FUNCTIONAL_INDIRECT_INDEX,
	A65_TEST_FUNCTIONAL_JUMP,
	A65_TEST_FUNCTIONAL_RELATIVE,
	A65_TEST_FUNCTIONAL_SIMPLE,
	A65_TEST_FUNCTIONAL_STACK,
	A65_TEST_FUNCTIONAL_SUBROUTINE,
};

#define A65_TEST_MAX A65_TEST_FUNCTIONAL_MAX

#define A65_TEST_FUNCTIONAL_MAX A65_TEST_FUNCTIONAL_SUBROUTINE
#define A65_TEST_FUNCTIONAL_MIN A65_TEST_FUNCTIONAL_ADVANCED

static const std::string A65_TEST_DIR[] = {
	"./functional/advanced/",
	"./functional/branch/",
	"./functional/index_indirect/",
	"./functional/indirect/",
	"./functional/indirect_index/",
	"./functional/jump/",
	"./functional/relative/",
	"./functional/simple/",
	"./functional/stack/",
	"./functional/subroutine/",
	};

#define A65_TEST_DIRECTORY(_TYPE_) \
	(((_TYPE_) > A65_TEST_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_TEST_DIR[_TYPE_]))

static const std::string A65_TEST_STR[] = {
	"Test::Functional::Advanced",
	"Test::Functional::Branch",
	"Test::Functional::Index-Indirect",
	"Test::Functional::Indirect",
	"Test::Functional::Indirect-Index",
	"Test::Functional::Jump",
	"Test::Functional::Relative",
	"Test::Functional::Simple",
	"Test::Functional::Stack",
	"Test::Functional::Subroutine",
	};

#define A65_TEST_STRING(_TYPE_) \
	(((_TYPE_) > A65_TEST_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_TEST_STR[_TYPE_]))

#endif // A65_TEST_TYPE_H_
