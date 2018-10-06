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
DIR_BIN_INC=./../bin/inc/
DIR_BIN_LIB=./../bin/lib/
DIR_BUILD=./../build/
DIR_INC=./../inc/
DIR_SRC=./
LIB=liba65.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN_LIB)$(LIB) $(DIR_BUILD)a65.o $(DIR_BUILD)a65_assembler.o $(DIR_BUILD)a65_lexer.o $(DIR_BUILD)a65_node.o $(DIR_BUILD)a65_object.o \
		$(DIR_BUILD)a65_parser.o $(DIR_BUILD)a65_section.o $(DIR_BUILD)a65_stream.o $(DIR_BUILD)a65_token.o $(DIR_BUILD)a65_tree.o \
		$(DIR_BUILD)a65_utility.o $(DIR_BUILD)a65_uuid.o
	cp $(DIR_INC)a65.h $(DIR_BIN_INC)
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_a65

### A65 ###

build_a65: a65.o a65_assembler.o a65_lexer.o a65_node.o a65_object.o a65_parser.o a65_section.o a65_stream.o a65_token.o a65_tree.o a65_utility.o \
	a65_uuid.o

a65.o: $(DIR_SRC)a65.cpp $(DIR_INC)a65.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65.cpp -o $(DIR_BUILD)a65.o

a65_assembler.o: $(DIR_SRC)a65_assembler.cpp $(DIR_INC)a65_assembler.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_assembler.cpp -o $(DIR_BUILD)a65_assembler.o

a65_lexer.o: $(DIR_SRC)a65_lexer.cpp $(DIR_INC)a65_lexer.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_lexer.cpp -o $(DIR_BUILD)a65_lexer.o

a65_node.o: $(DIR_SRC)a65_node.cpp $(DIR_INC)a65_node.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_node.cpp -o $(DIR_BUILD)a65_node.o

a65_object.o: $(DIR_SRC)a65_object.cpp $(DIR_INC)a65_object.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_object.cpp -o $(DIR_BUILD)a65_object.o

a65_parser.o: $(DIR_SRC)a65_parser.cpp $(DIR_INC)a65_parser.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_parser.cpp -o $(DIR_BUILD)a65_parser.o

a65_section.o: $(DIR_SRC)a65_section.cpp $(DIR_INC)a65_section.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_section.cpp -o $(DIR_BUILD)a65_section.o

a65_stream.o: $(DIR_SRC)a65_stream.cpp $(DIR_INC)a65_stream.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_stream.cpp -o $(DIR_BUILD)a65_stream.o

a65_token.o: $(DIR_SRC)a65_token.cpp $(DIR_INC)a65_token.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_token.cpp -o $(DIR_BUILD)a65_token.o

a65_tree.o: $(DIR_SRC)a65_tree.cpp $(DIR_INC)a65_tree.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_tree.cpp -o $(DIR_BUILD)a65_tree.o

a65_utility.o: $(DIR_SRC)a65_utility.cpp $(DIR_INC)a65_utility.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_utility.cpp -o $(DIR_BUILD)a65_utility.o

a65_uuid.o: $(DIR_SRC)a65_uuid.cpp $(DIR_INC)a65_uuid.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)a65_uuid.cpp -o $(DIR_BUILD)a65_uuid.o
