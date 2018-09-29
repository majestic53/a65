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

#ifndef A65_DEFINE_H_
#define A65_DEFINE_H_

#include <algorithm>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef __in
#define __in
#endif // __in
#ifndef __in_opt
#define __in_opt
#endif // __in_opt
#ifndef __inout
#define __inout
#endif // __inout
#ifndef __inout_opt
#define __inout_opt
#endif // __inout_opt

#define A65_FLOAT_PREC(_PREC_, _VALUE_) \
	std::right << std::setprecision(_PREC_) << std::fixed << (_VALUE_) << std::defaultfloat

#define A65_STRING_EMPTY "<Empty>"
#define A65_STRING_MALFORMED "<Malformed>"
#define A65_STRING_UNKNOWN "<Unknown>"

#define A65_STRING_CHECK(_STRING_) \
	((_STRING_).empty() ? A65_STRING_EMPTY : (_STRING_).c_str())

#define _A65_STRING_CONCAT(_STRING_) \
	# _STRING_
#define A65_STRING_CONCAT(_STRING_) \
	_A65_STRING_CONCAT(_STRING_)

#define A65_STRING_HEX(_TYPE_, _VALUE_) \
	std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex << (uintmax_t) ((_TYPE_) (_VALUE_)) \
		<< std::dec << std::setfill(' ')

#define A65_STRING_LOWER(_STRING_) \
	std::transform((_STRING_).begin(), (_STRING_).end(), (_STRING_).begin(), ::tolower)

#define A65_VERSION_MAJOR 0
#define A65_VERSION_MINOR 1
#define A65_VERSION_REVISION 25

#endif // A65_DEFINE_H_
