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

#ifndef A65_ASSEMBLER_TYPE_H_
#define A65_ASSEMBLER_TYPE_H_

#include "./a65.h"
#include "./a65_token_type.h"

#define A65_ASSEMBLER_CHARACTER_METADATA '@'
#define A65_ASSEMBLER_CHARACTER_TAB '\t'

#define A65_ASSEMBLER_DIVIDER "============================================"

#define A65_ASSEMBLER_FILL 0xea
#define A65_ASSEMBLER_FILL_RESERVE 0x00

#define A65_ASSEMBLER_OUTPUT_ARCHIVE_NAME_DEFAULT "a"
#define A65_ASSEMBLER_OUTPUT_ARCHIVE_EXTENSION ".a"

#define A65_ASSEMBLER_OUTPUT_BINARY_NAME_DEFAULT "b"
#define A65_ASSEMBLER_OUTPUT_BINARY_EXTENSION ".bin"

#define A65_ASSEMBLER_OUTPUT_IHEX_NAME_DEFAULT "i"
#define A65_ASSEMBLER_OUTPUT_IHEX_EXTENSION ".hex"

#define A65_ASSEMBLER_OUTPUT_OBJECT_NAME_DEFAULT "o"
#define A65_ASSEMBLER_OUTPUT_OBJECT_EXTENSION ".o"

#define A65_ASSEMBLER_OUTPUT_PATH_DEFAULT "./"
#define A65_ASSEMBLER_OUTPUT_SEPERATOR '/'

#define A65_ASSEMBLER_OUTPUT_SOURCE_NAME_DEFAULT "s"
#define A65_ASSEMBLER_OUTPUT_SOURCE_EXTENSION ".S"

#define A65_ASSEMBLER_SECTION_NAME_DEFAULT "sec"

enum {
	A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE = 1,
	A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE,
	A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD
};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x6d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x2d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x0e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x2c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xcd, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_CPX, std::make_pair(0xec, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_CPY, std::make_pair(0xcc, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xce, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x4d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xee, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_JMP, std::make_pair(0x4c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_JSR, std::make_pair(0x20, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xad, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xae, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xac, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x4e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x0d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x2e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x6e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xed, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x8d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STX, std::make_pair(0x8e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STY, std::make_pair(0x8c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x9c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_TRB, std::make_pair(0x1c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_TSB, std::make_pair(0x0c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	};

#define A65_IS_COMMAND_ABSOLUTE(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP.end())

#define A65_COMMAND_ABSOLUTE_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ABSOLUTE_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_JMP, std::make_pair(0x7c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	};

#define A65_IS_COMMAND_ABSOLUTE_INDEX_INDIRECT(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP.end())

#define A65_COMMAND_ABSOLUTE_INDEX_INDIRECT_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ABSOLUTE_INDEX_INDIRECT_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x7d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x3d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x1e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x3c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xdd, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xde, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x5d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xfe, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xbd, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xbc, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x5e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x1d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x3e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x7e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xfd, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x9d, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x9e, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	};

#define A65_IS_COMMAND_ABSOLUTE_INDEX_X(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP.end())

#define A65_COMMAND_ABSOLUTE_INDEX_X_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ABSOLUTE_INDEX_X_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x79, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x39, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd9, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x59, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb9, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xbe, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x19, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf9, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x99, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	};

#define A65_IS_COMMAND_ABSOLUTE_INDEX_Y(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP.end())

#define A65_COMMAND_ABSOLUTE_INDEX_Y_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ABSOLUTE_INDEX_Y_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_JMP, std::make_pair(0x6c, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	};

#define A65_IS_COMMAND_ABSOLUTE_INDIRECT(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP.end())

#define A65_COMMAND_ABSOLUTE_INDIRECT_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ABSOLUTE_INDIRECT_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x0a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0x3a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0x1a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x4a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x2a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x6a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	};

#define A65_IS_COMMAND_ACCUMULATOR(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP.end())

#define A65_COMMAND_ACCUMULATOR_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ACCUMULATOR_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x69, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x29, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x89, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xc9, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CPX, std::make_pair(0xe0, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CPY, std::make_pair(0xc0, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x49, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xa9, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xa2, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xa0, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x09, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xe9, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_IMMEDIATE(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP.end())

#define A65_COMMAND_IMMEDIATE_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_IMMEDIATE_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_IMPLIED_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_BRK, std::make_pair(0x00, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CLC, std::make_pair(0x18, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_CLD, std::make_pair(0xd8, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_CLI, std::make_pair(0x58, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_CLV, std::make_pair(0xb8, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_DEX, std::make_pair(0xca, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_DEY, std::make_pair(0x88, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_INX, std::make_pair(0xe8, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_INY, std::make_pair(0xc8, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_NOP, std::make_pair(0xea, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PHA, std::make_pair(0x48, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PHP, std::make_pair(0x08, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PHX, std::make_pair(0xda, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PHY, std::make_pair(0x5a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PLA, std::make_pair(0x68, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PLP, std::make_pair(0x28, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PLX, std::make_pair(0xfa, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_PLY, std::make_pair(0x7a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_RTI, std::make_pair(0x40, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_RTS, std::make_pair(0x60, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_SEC, std::make_pair(0x38, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_SED, std::make_pair(0xf8, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_SEI, std::make_pair(0x78, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_STP, std::make_pair(0xdb, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_TAX, std::make_pair(0xaa, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_TAY, std::make_pair(0xa8, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_TSX, std::make_pair(0xba, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_TXA, std::make_pair(0x8a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_TXS, std::make_pair(0x9a, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_TYA, std::make_pair(0x98, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	std::make_pair(A65_TOKEN_COMMAND_WAI, std::make_pair(0xcb, A65_ASSEMBLER_COMMAND_IMMEDIATE_NONE)),
	};

#define A65_IS_COMMAND_IMPLIED(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_IMPLIED_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_IMPLIED_MAP.end())

#define A65_COMMAND_IMPLIED_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_IMPLIED_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_IMPLIED_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_IMPLIED_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_RELATIVE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_BBR0, std::make_pair(0x0f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR1, std::make_pair(0x1f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR2, std::make_pair(0x2f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR3, std::make_pair(0x3f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR4, std::make_pair(0x4f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR5, std::make_pair(0x5f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR6, std::make_pair(0x6f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBR7, std::make_pair(0x7f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS0, std::make_pair(0x8f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS1, std::make_pair(0x9f, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS2, std::make_pair(0xaf, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS3, std::make_pair(0xbf, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS4, std::make_pair(0xcf, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS5, std::make_pair(0xdf, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS6, std::make_pair(0xef, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BBS7, std::make_pair(0xff, A65_ASSEMBLER_COMMAND_IMMEDIATE_WORD)),
	std::make_pair(A65_TOKEN_COMMAND_BCC, std::make_pair(0x90, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BCS, std::make_pair(0xb0, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BEQ, std::make_pair(0xf0, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BMI, std::make_pair(0x30, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BNE, std::make_pair(0xd0, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BPL, std::make_pair(0x10, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BRA, std::make_pair(0x80, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BVC, std::make_pair(0x50, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BVS, std::make_pair(0x70, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_RELATIVE(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_RELATIVE_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_RELATIVE_MAP.end())

#define A65_COMMAND_RELATIVE_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_RELATIVE_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_RELATIVE_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_RELATIVE_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x65, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x25, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x06, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x24, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xc5, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CPX, std::make_pair(0xe4, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CPY, std::make_pair(0xc4, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xc6, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x45, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xe6, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xa5, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xa6, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xa4, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x46, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x05, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB0, std::make_pair(0x07, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB1, std::make_pair(0x17, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB2, std::make_pair(0x27, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB3, std::make_pair(0x37, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB4, std::make_pair(0x47, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB5, std::make_pair(0x57, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB6, std::make_pair(0x67, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_RMB7, std::make_pair(0x77, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x26, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x66, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xe5, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB0, std::make_pair(0x87, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB1, std::make_pair(0x97, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB2, std::make_pair(0xa7, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB3, std::make_pair(0xb7, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB4, std::make_pair(0xc7, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB5, std::make_pair(0xd7, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB6, std::make_pair(0xe7, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SMB7, std::make_pair(0xf7, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x85, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STX, std::make_pair(0x86, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STY, std::make_pair(0x84, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x64, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_TRB, std::make_pair(0x14, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_TSB, std::make_pair(0x04, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_ZEROPAGE(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP.end())

#define A65_COMMAND_ZEROPAGE_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ZEROPAGE_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x61, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x21, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xc1, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x41, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xa1, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x01, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xe1, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x81, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_ZEROPAGE_INDEX_INDIRECT(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP.end())

#define A65_COMMAND_ZEROPAGE_INDEX_INDIRECT_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ZEROPAGE_INDEX_INDIRECT_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x75, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x35, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x16, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x34, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd5, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xd6, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x55, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xf6, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb5, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xb4, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x56, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x15, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x36, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x76, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf5, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x95, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STY, std::make_pair(0x94, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x74, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_ZEROPAGE_INDEX_X(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP.end())

#define A65_COMMAND_ZEROPAGE_INDEX_X_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ZEROPAGE_INDEX_X_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xb6, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STX, std::make_pair(0x96, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_ZEROPAGE_INDEX_Y(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP.end())

#define A65_COMMAND_ZEROPAGE_INDEX_Y_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ZEROPAGE_INDEX_Y_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x72, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x32, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd2, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x52, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb2, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x12, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf2, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x92, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_ZEROPAGE_INDIRECT(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP.end())

#define A65_COMMAND_ZEROPAGE_INDIRECT_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ZEROPAGE_INDIRECT_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP.find(_TYPE_)->second.second

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x71, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x31, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd1, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x51, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb1, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x11, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf1, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x91, A65_ASSEMBLER_COMMAND_IMMEDIATE_BYTE)),
	};

#define A65_IS_COMMAND_ZEROPAGE_INDIRECT_INDEX(_TYPE_) \
	(A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP.find(_TYPE_) != A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP.end())

#define A65_COMMAND_ZEROPAGE_INDIRECT_INDEX_OPCODE(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP.find(_TYPE_)->second.first

#define A65_COMMAND_ZEROPAGE_INDIRECT_INDEX_LENGTH(_TYPE_) \
	A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP.find(_TYPE_)->second.second

static const std::map<int, std::map<int, std::pair<uint8_t, size_t>>> A65_ASSEMBLER_COMMAND_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_MODE_ABSOLUTE, A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_INDIRECT, A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_X, A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_Y, A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDIRECT, A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ACCUMULATOR, A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_IMMEDIATE, A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_IMPLIED, A65_ASSEMBLER_COMMAND_IMPLIED_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_RELATIVE, A65_ASSEMBLER_COMMAND_RELATIVE_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ZEROPAGE, A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_INDIRECT, A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_X, A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_Y, A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT, A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP),
	std::make_pair(A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT_INDEX, A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP),
	};

#define A65_ASSEMBLER_COMMAND_OPCODE(_TYPE_, _MODE_) \
	A65_ASSEMBLER_COMMAND_MAP.find(_MODE_)->second.find(_TYPE_)->second.first

#define A65_ASSEMBLER_COMMAND_LENGTH(_TYPE_, _MODE_) \
	A65_ASSEMBLER_COMMAND_MAP.find(_MODE_)->second.find(_TYPE_)->second.second

#define A65_EXTENSION "."

enum {
	A65_IHEX_DATA = 0,
	A65_IHEX_END,
};

#define A65_IHEX_CHARACTER ':'

#define A65_IHEX_LENGTH 16

#define A65_IHEX_MAX A65_IHEX_END

static const std::string A65_IHEX_STR[] = {
	"Data", "End",
	};

#define A65_IHEX_STRING(_TYPE_) \
	(((_TYPE_) > A65_IHEX_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_IHEX_STR[_TYPE_]))

#endif // A65_ASSEMBLER_TYPE_H_
