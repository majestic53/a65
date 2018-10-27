# A65
# Copyright (C) 2018 David Jolly
#
# A65 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# A65 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
DIR_BIN_LIB=./../bin/lib/
DIR_BUILD=./../build/
DIR_BUILD_TEST=./../build/test/
DIR_FUNCTIONAL=./functional/
DIR_TEST=./
EXE=a65_test
LIB=liba65.a

all: exe

### EXECUTABLE ###

exe:
	@echo ''
	@echo '--- BUILDING EXECUTABLE --------------------'
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) $(DIR_TEST)$(EXE).cpp $(DIR_BIN_LIB)$(LIB) -o $(DIR_BUILD_TEST)$(EXE)
	cp -r $(DIR_FUNCTIONAL) $(DIR_BUILD_TEST)
	@echo '--- DONE -----------------------------------'
	@echo ''
