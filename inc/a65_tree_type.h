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

#ifndef A65_TREE_TYPE_H_
#define A65_TREE_TYPE_H_

#include "./a65_define.h"

enum {
	A65_TREE_BEGIN = 0,
	A65_TREE_END,
	A65_TREE_STATEMENT,
};

#define A65_TREE_MAX A65_TREE_STATEMENT

static const std::string A65_TREE_STR[] = {
	"Begin", "End", "Statement",
	};

#define A65_TREE_STRING(_TYPE_) \
	(((_TYPE_) > A65_TREE_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_TREE_STR[_TYPE_]))

#endif // A65_TREE_TYPE_H_
