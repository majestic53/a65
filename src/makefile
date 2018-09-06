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
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../inc/
DIR_SRC=./
LIB=liba65.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)a65.o $(DIR_BUILD)a65_stream.o $(DIR_BUILD)a65_token.o $(DIR_BUILD)a65_utility.o $(DIR_BUILD)a65_uuid.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_a65

### A65 ###

build_a65: a65.o a65_stream.o a65_token.o a65_utility.o a65_uuid.o

a65.o: $(DIR_SRC)a65.cpp $(DIR_INC)a65.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65.cpp -o $(DIR_BUILD)a65.o

a65_stream.o: $(DIR_SRC)a65_stream.cpp $(DIR_INC)a65_stream.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_stream.cpp -o $(DIR_BUILD)a65_stream.o

a65_token.o: $(DIR_SRC)a65_token.cpp $(DIR_INC)a65_token.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_token.cpp -o $(DIR_BUILD)a65_token.o

a65_utility.o: $(DIR_SRC)a65_utility.cpp $(DIR_INC)a65_utility.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_utility.cpp -o $(DIR_BUILD)a65_utility.o

a65_uuid.o: $(DIR_SRC)a65_uuid.cpp $(DIR_INC)a65_uuid.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_uuid.cpp -o $(DIR_BUILD)a65_uuid.o
