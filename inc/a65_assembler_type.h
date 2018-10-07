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

#define A65_ASSEMBLER_DIVIDER "============================================"

#define A65_ASSEMBLER_EXIT "[Exiting]"

#define A65_ASSEMBLER_OUTPUT_PATH_DEFAULT "./"
#define A65_ASSEMBLER_OUTPUT_SEPERATOR '/'

#define A65_ASSEMBLER_OUTPUT_OBJECT_NAME_DEFAULT "obj"
#define A65_ASSEMBLER_OUTPUT_OBJECT_EXTENSION ".o"

#define A65_ASSEMBLER_OUTPUT_SOURCE_NAME_DEFAULT "src"
#define A65_ASSEMBLER_OUTPUT_SOURCE_EXTENSION ".S"

#define A65_ASSEMBLER_SECTION_DONE "[Done]"
#define A65_ASSEMBLER_SECTION_EVALUATE "Assembling object"
#define A65_ASSEMBLER_SECTION_OBJECT "Generating object file"
#define A65_ASSEMBLER_SECTION_PREPROCESS "Proprocessing input file"
#define A65_ASSEMBLER_SECTION_SOURCE "Generating source file"

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x6d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x2d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x0e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x2c, 3)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xcd, 3)),
	std::make_pair(A65_TOKEN_COMMAND_CPX, std::make_pair(0xec, 3)),
	std::make_pair(A65_TOKEN_COMMAND_CPY, std::make_pair(0xcc, 3)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xce, 3)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x4d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xee, 3)),
	std::make_pair(A65_TOKEN_COMMAND_JMP, std::make_pair(0x4c, 3)),
	std::make_pair(A65_TOKEN_COMMAND_JSR, std::make_pair(0x20, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xad, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xae, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xac, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x4e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x0d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x2e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x6e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xed, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x8d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STX, std::make_pair(0x8e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STY, std::make_pair(0x8c, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x9c, 3)),
	std::make_pair(A65_TOKEN_COMMAND_TRB, std::make_pair(0x1c, 3)),
	std::make_pair(A65_TOKEN_COMMAND_TSB, std::make_pair(0x0c, 3)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_JMP, std::make_pair(0x7c, 3)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_X_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x7d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x3d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x1e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x3c, 3)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xdd, 3)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xde, 3)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x5d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xfe, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xbd, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xbc, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x5e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x1d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x3e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x7e, 3)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xfd, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x9d, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x9e, 3)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDEX_Y_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x79, 3)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x39, 3)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd9, 3)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x59, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb9, 3)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xbe, 3)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x19, 3)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf9, 3)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x99, 3)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ABSOLUTE_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_JMP, std::make_pair(0x6c, 3)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ACCUMULATOR_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x0a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0x3a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0x1a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x4a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x2a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x6a, 1)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_IMMEDIATE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x69, 2)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x29, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x89, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xc9, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CPX, std::make_pair(0xe0, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CPY, std::make_pair(0xc0, 2)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x49, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xa9, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xa2, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xa0, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x09, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xe9, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_IMPLIED_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_BRK, std::make_pair(0x00, 1)),
	std::make_pair(A65_TOKEN_COMMAND_CLC, std::make_pair(0x18, 1)),
	std::make_pair(A65_TOKEN_COMMAND_CLD, std::make_pair(0xd8, 1)),
	std::make_pair(A65_TOKEN_COMMAND_CLI, std::make_pair(0x58, 1)),
	std::make_pair(A65_TOKEN_COMMAND_CLV, std::make_pair(0xb8, 1)),
	std::make_pair(A65_TOKEN_COMMAND_DEX, std::make_pair(0xca, 1)),
	std::make_pair(A65_TOKEN_COMMAND_DEY, std::make_pair(0x88, 1)),
	std::make_pair(A65_TOKEN_COMMAND_INX, std::make_pair(0xe8, 1)),
	std::make_pair(A65_TOKEN_COMMAND_INY, std::make_pair(0xc8, 1)),
	std::make_pair(A65_TOKEN_COMMAND_NOP, std::make_pair(0xea, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PHA, std::make_pair(0x48, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PHP, std::make_pair(0x08, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PHX, std::make_pair(0xda, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PHY, std::make_pair(0x5a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PLA, std::make_pair(0x68, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PLP, std::make_pair(0x28, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PLX, std::make_pair(0xfa, 1)),
	std::make_pair(A65_TOKEN_COMMAND_PLY, std::make_pair(0x7a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_RTI, std::make_pair(0x40, 1)),
	std::make_pair(A65_TOKEN_COMMAND_RTS, std::make_pair(0x60, 1)),
	std::make_pair(A65_TOKEN_COMMAND_SEC, std::make_pair(0x38, 1)),
	std::make_pair(A65_TOKEN_COMMAND_SED, std::make_pair(0xf8, 1)),
	std::make_pair(A65_TOKEN_COMMAND_SEI, std::make_pair(0x78, 1)),
	std::make_pair(A65_TOKEN_COMMAND_STP, std::make_pair(0xdb, 1)),
	std::make_pair(A65_TOKEN_COMMAND_TAX, std::make_pair(0xaa, 1)),
	std::make_pair(A65_TOKEN_COMMAND_TAY, std::make_pair(0xa8, 1)),
	std::make_pair(A65_TOKEN_COMMAND_TSX, std::make_pair(0xba, 1)),
	std::make_pair(A65_TOKEN_COMMAND_TXA, std::make_pair(0x8a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_TXS, std::make_pair(0x9a, 1)),
	std::make_pair(A65_TOKEN_COMMAND_TYA, std::make_pair(0x98, 1)),
	std::make_pair(A65_TOKEN_COMMAND_WAI, std::make_pair(0xcb, 1)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_RELATIVE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_BBR0, std::make_pair(0x0f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR1, std::make_pair(0x1f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR2, std::make_pair(0x2f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR3, std::make_pair(0x3f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR4, std::make_pair(0x4f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR5, std::make_pair(0x5f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR6, std::make_pair(0x6f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBR7, std::make_pair(0x7f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS0, std::make_pair(0x8f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS1, std::make_pair(0x9f, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS2, std::make_pair(0xaf, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS3, std::make_pair(0xbf, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS4, std::make_pair(0xcf, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS5, std::make_pair(0xdf, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS6, std::make_pair(0xef, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BBS7, std::make_pair(0xff, 3)),
	std::make_pair(A65_TOKEN_COMMAND_BCC, std::make_pair(0x90, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BCS, std::make_pair(0xb0, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BEQ, std::make_pair(0xf0, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BMI, std::make_pair(0x30, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BNE, std::make_pair(0xd0, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BPL, std::make_pair(0x10, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BRA, std::make_pair(0x80, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BVC, std::make_pair(0x50, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BVS, std::make_pair(0x70, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x65, 2)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x25, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x06, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x24, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xc5, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CPX, std::make_pair(0xe4, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CPY, std::make_pair(0xc4, 2)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xc6, 2)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x45, 2)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xe6, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xa5, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xa6, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xa4, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x46, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x05, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB0, std::make_pair(0x07, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB1, std::make_pair(0x17, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB2, std::make_pair(0x27, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB3, std::make_pair(0x37, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB4, std::make_pair(0x47, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB5, std::make_pair(0x57, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB6, std::make_pair(0x67, 2)),
	std::make_pair(A65_TOKEN_COMMAND_RMB7, std::make_pair(0x77, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x26, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x66, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xe5, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB0, std::make_pair(0x87, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB1, std::make_pair(0x97, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB2, std::make_pair(0xa7, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB3, std::make_pair(0xb7, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB4, std::make_pair(0xc7, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB5, std::make_pair(0xd7, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB6, std::make_pair(0xe7, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SMB7, std::make_pair(0xf7, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x85, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STX, std::make_pair(0x86, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STY, std::make_pair(0x84, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x64, 2)),
	std::make_pair(A65_TOKEN_COMMAND_TRB, std::make_pair(0x14, 2)),
	std::make_pair(A65_TOKEN_COMMAND_TSB, std::make_pair(0x04, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x61, 2)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x21, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xc1, 2)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x41, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xa1, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x01, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xe1, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x81, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_X_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x75, 2)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x35, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ASL, std::make_pair(0x16, 2)),
	std::make_pair(A65_TOKEN_COMMAND_BIT, std::make_pair(0x34, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd5, 2)),
	std::make_pair(A65_TOKEN_COMMAND_DEC, std::make_pair(0xd6, 2)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x55, 2)),
	std::make_pair(A65_TOKEN_COMMAND_INC, std::make_pair(0xf6, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb5, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDY, std::make_pair(0xb4, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LSR, std::make_pair(0x56, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x15, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ROL, std::make_pair(0x36, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ROR, std::make_pair(0x76, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf5, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x95, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STY, std::make_pair(0x94, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STZ, std::make_pair(0x74, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDEX_Y_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_LDX, std::make_pair(0xb6, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STX, std::make_pair(0x96, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x72, 2)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x32, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd2, 2)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x52, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb2, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x12, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf2, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x92, 2)),
	};

static const std::map<int, std::pair<uint8_t, size_t>> A65_ASSEMBLER_COMMAND_ZEROPAGE_INDIRECT_INDEX_MAP = {
	std::make_pair(A65_TOKEN_COMMAND_ADC, std::make_pair(0x71, 2)),
	std::make_pair(A65_TOKEN_COMMAND_AND, std::make_pair(0x31, 2)),
	std::make_pair(A65_TOKEN_COMMAND_CMP, std::make_pair(0xd1, 2)),
	std::make_pair(A65_TOKEN_COMMAND_EOR, std::make_pair(0x51, 2)),
	std::make_pair(A65_TOKEN_COMMAND_LDA, std::make_pair(0xb1, 2)),
	std::make_pair(A65_TOKEN_COMMAND_ORA, std::make_pair(0x11, 2)),
	std::make_pair(A65_TOKEN_COMMAND_SBC, std::make_pair(0xf1, 2)),
	std::make_pair(A65_TOKEN_COMMAND_STA, std::make_pair(0x91, 2)),
	};

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

#endif // A65_ASSEMBLER_TYPE_H_
