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

#ifndef A65_TOKEN_H_
#define A65_TOKEN_H_

#include "./a65_stream.h"
#include "./a65_uuid.h"

#define A65_TOKEN_MODE_UNDEFINED (-1)
#define A65_TOKEN_SUBTYPE_UNDEFINED (-1)

enum {
	A65_TOKEN_BEGIN = 0,
	A65_TOKEN_COMMAND,
	A65_TOKEN_CONSTANT,
	A65_TOKEN_DIRECTIVE,
	A65_TOKEN_END,
	A65_TOKEN_IDENTIFIER,
	A65_TOKEN_LABEL,
	A65_TOKEN_LITERAL,
	A65_TOKEN_MACRO,
	A65_TOKEN_PRAGMA,
	A65_TOKEN_REGISTER,
	A65_TOKEN_SCALAR,
	A65_TOKEN_SYMBOL,
};

#define A65_TOKEN_MAX A65_TOKEN_SYMBOL

static const std::string A65_TOKEN_STR[] = {
	"Begin", "Command", "Constant", "Directive", "End", "Identifier", "Label", "Literal",
	"Macro", "Pragma", "Register", "Scalar", "Symbol",
	};

#define A65_TOKEN_STRING(_TYPE_) \
	(((_TYPE_) > A65_TOKEN_MAX) ? A65_STRING_UNKNOWN : \
		A65_STRING_CHECK(A65_TOKEN_STR[_TYPE_]))

enum {
	A65_TOKEN_COMMAND_ADC = 0,
	A65_TOKEN_COMMAND_AND,
	A65_TOKEN_COMMAND_ASL,
	A65_TOKEN_COMMAND_BBR0,
	A65_TOKEN_COMMAND_BBR1,
	A65_TOKEN_COMMAND_BBR2,
	A65_TOKEN_COMMAND_BBR3,
	A65_TOKEN_COMMAND_BBR4,
	A65_TOKEN_COMMAND_BBR5,
	A65_TOKEN_COMMAND_BBR6,
	A65_TOKEN_COMMAND_BBR7,
	A65_TOKEN_COMMAND_BBS0,
	A65_TOKEN_COMMAND_BBS1,
	A65_TOKEN_COMMAND_BBS2,
	A65_TOKEN_COMMAND_BBS3,
	A65_TOKEN_COMMAND_BBS4,
	A65_TOKEN_COMMAND_BBS5,
	A65_TOKEN_COMMAND_BBS6,
	A65_TOKEN_COMMAND_BBS7,
	A65_TOKEN_COMMAND_BCC,
	A65_TOKEN_COMMAND_BCS,
	A65_TOKEN_COMMAND_BEQ,
	A65_TOKEN_COMMAND_BIT,
	A65_TOKEN_COMMAND_BMI,
	A65_TOKEN_COMMAND_BNE,
	A65_TOKEN_COMMAND_BPL,
	A65_TOKEN_COMMAND_BRA,
	A65_TOKEN_COMMAND_BRK,
	A65_TOKEN_COMMAND_BVC,
	A65_TOKEN_COMMAND_BVS,
	A65_TOKEN_COMMAND_CLC,
	A65_TOKEN_COMMAND_CLD,
	A65_TOKEN_COMMAND_CLI,
	A65_TOKEN_COMMAND_CLV,
	A65_TOKEN_COMMAND_CMP,
	A65_TOKEN_COMMAND_CPX,
	A65_TOKEN_COMMAND_CPY,
	A65_TOKEN_COMMAND_DEC,
	A65_TOKEN_COMMAND_DEX,
	A65_TOKEN_COMMAND_DEY,
	A65_TOKEN_COMMAND_EOR,
	A65_TOKEN_COMMAND_INC,
	A65_TOKEN_COMMAND_INX,
	A65_TOKEN_COMMAND_INY,
	A65_TOKEN_COMMAND_JMP,
	A65_TOKEN_COMMAND_JSR,
	A65_TOKEN_COMMAND_LDA,
	A65_TOKEN_COMMAND_LDX,
	A65_TOKEN_COMMAND_LDY,
	A65_TOKEN_COMMAND_LSR,
	A65_TOKEN_COMMAND_NOP,
	A65_TOKEN_COMMAND_ORA,
	A65_TOKEN_COMMAND_PHA,
	A65_TOKEN_COMMAND_PHP,
	A65_TOKEN_COMMAND_PHX,
	A65_TOKEN_COMMAND_PHY,
	A65_TOKEN_COMMAND_PLA,
	A65_TOKEN_COMMAND_PLP,
	A65_TOKEN_COMMAND_PLX,
	A65_TOKEN_COMMAND_PLY,
	A65_TOKEN_COMMAND_RMB0,
	A65_TOKEN_COMMAND_RMB1,
	A65_TOKEN_COMMAND_RMB2,
	A65_TOKEN_COMMAND_RMB3,
	A65_TOKEN_COMMAND_RMB4,
	A65_TOKEN_COMMAND_RMB5,
	A65_TOKEN_COMMAND_RMB6,
	A65_TOKEN_COMMAND_RMB7,
	A65_TOKEN_COMMAND_ROL,
	A65_TOKEN_COMMAND_ROR,
	A65_TOKEN_COMMAND_RTI,
	A65_TOKEN_COMMAND_RTS,
	A65_TOKEN_COMMAND_SMB,
	A65_TOKEN_COMMAND_SEC,
	A65_TOKEN_COMMAND_SED,
	A65_TOKEN_COMMAND_SEI,
	A65_TOKEN_COMMAND_SMB0,
	A65_TOKEN_COMMAND_SMB1,
	A65_TOKEN_COMMAND_SMB2,
	A65_TOKEN_COMMAND_SMB3,
	A65_TOKEN_COMMAND_SMB4,
	A65_TOKEN_COMMAND_SMB5,
	A65_TOKEN_COMMAND_SMB6,
	A65_TOKEN_COMMAND_SMB7,
	A65_TOKEN_COMMAND_STA,
	A65_TOKEN_COMMAND_STP,
	A65_TOKEN_COMMAND_STX,
	A65_TOKEN_COMMAND_STY,
	A65_TOKEN_COMMAND_STZ,
	A65_TOKEN_COMMAND_TAX,
	A65_TOKEN_COMMAND_TAY,
	A65_TOKEN_COMMAND_TRB,
	A65_TOKEN_COMMAND_TSB,
	A65_TOKEN_COMMAND_TSX,
	A65_TOKEN_COMMAND_TXA,
	A65_TOKEN_COMMAND_TXS,
	A65_TOKEN_COMMAND_TYA,
	A65_TOKEN_COMMAND_WAI,
};

enum {
	A65_TOKEN_COMMAND_MODE_ABSOLUTE = 0,
	A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_INDIRECT,
	A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_X,
	A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDEX_Y,
	A65_TOKEN_COMMAND_MODE_ABSOLUTE_INDIRECT,
	A65_TOKEN_COMMAND_MODE_ACCUMULATOR,
	A65_TOKEN_COMMAND_MODE_IMMEDIATE,
	A65_TOKEN_COMMAND_MODE_IMPLIED,
	A65_TOKEN_COMMAND_MODE_RELATIVE,
	A65_TOKEN_COMMAND_MODE_ZEROPAGE,
	A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_INDIRECT,
	A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_X,
	A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDEX_Y,
	A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT,
	A65_TOKEN_COMMAND_MODE_ZEROPAGE_INDIRECT_INDEX,
};

enum {
	A65_TOKEN_CONSTANT_FALSE = 0,
	A65_TOKEN_CONSTANT_NULL,
	A65_TOKEN_CONSTANT_TRUE,
};

enum {
	A65_TOKEN_DIRECTIVE_DATA_BYTE = 0,
	A65_TOKEN_DIRECTIVE_DATA_WORD,
	A65_TOKEN_DIRECTIVE_DEFINE,
	A65_TOKEN_DIRECTIVE_ELSE,
	A65_TOKEN_DIRECTIVE_ELSE_IF,
	A65_TOKEN_DIRECTIVE_END,
	A65_TOKEN_DIRECTIVE_IF,
	A65_TOKEN_DIRECTIVE_IF_DEFINE,
	A65_TOKEN_DIRECTIVE_ORIGIN,
	A65_TOKEN_DIRECTIVE_RESERVE,
	A65_TOKEN_DIRECTIVE_UNDEFINE,
};

enum {
	A65_TOKEN_MACRO_HIGH = 0,
	A65_TOKEN_MACRO_LOW,
};

enum {
	A65_TOKEN_PRAGMA_INCLUDE_BINARY = 0,
	A65_TOKEN_PRAGMA_INCLUDE_SOURCE,
};

enum {
	A65_TOKEN_REGISTER_ACCUMULATOR = 0,
	A65_TOKEN_REGISTER_INDEX_X,
	A65_TOKEN_REGISTER_INDEX_Y,
};

enum {
	A65_TOKEN_SYMBOL_ARITHMETIC_ADDITION,
	A65_TOKEN_SYMBOL_ARITHMETIC_DIVIDE,
	A65_TOKEN_SYMBOL_ARITHMETIC_MODULUS,
	A65_TOKEN_SYMBOL_ARITHMETIC_MULTIPLY,
	A65_TOKEN_SYMBOL_ARITHMETIC_SUBTRACTION,
	A65_TOKEN_SYMBOL_BINARY_AND,
	A65_TOKEN_SYMBOL_BINARY_OR,
	A65_TOKEN_SYMBOL_BINARY_XOR,
	A65_TOKEN_SYMBOL_LOGICAL_AND,
	A65_TOKEN_SYMBOL_LOGICAL_OR,
	A65_TOKEN_SYMBOL_LOGICAL_EQUALS,
	A65_TOKEN_SYMBOL_LOGICAL_GREATER_THAN,
	A65_TOKEN_SYMBOL_LOGICAL_GREATER_THAN_EQUALS,
	A65_TOKEN_SYMBOL_LOGICAL_LESS_THAN,
	A65_TOKEN_SYMBOL_LOGICAL_LESS_THAN_EQUALS,
	A65_TOKEN_SYMBOL_LOGICAL_NOT_EQUALS,
	A65_TOKEN_SYMBOL_LOGICAL_SHIFT_LEFT,
	A65_TOKEN_SYMBOL_LOGICAL_SHIFT_RIGHT,
	A65_TOKEN_SYMBOL_BRACE_CURLY_CLOSE,
	A65_TOKEN_SYMBOL_BRACE_CURLY_OPEN,
	A65_TOKEN_SYMBOL_BRACE_SQUARE_CLOSE,
	A65_TOKEN_SYMBOL_BRACE_SQUARE_OPEN,
	A65_TOKEN_SYMBOL_IMMEDIATE,
	A65_TOKEN_SYMBOL_PARENTHESIS_CLOSE,
	A65_TOKEN_SYMBOL_PARENTHESIS_OPEN,
	A65_TOKEN_SYMBOL_SEPERATOR,
	A65_TOKEN_SYMBOL_UNARY_NEGATION,
	A65_TOKEN_SYMBOL_UNARY_NOT,
};

class a65_token {

	public:

		a65_token(
			__in_opt int type = A65_TOKEN_BEGIN,
			__in_opt int subtype = A65_TOKEN_SUBTYPE_UNDEFINED,
			__in_opt int mode = A65_TOKEN_MODE_UNDEFINED
			);

		a65_token(
			__in const a65_token &other
			);

		virtual ~a65_token(void);

		a65_token &operator=(
			__in const a65_token &other
			);

		a65_uuid_t id(void) const;

		size_t line(void) const;

		a65_literal_t literal(void) const;

		bool match(
			__in int type,
			__in_opt int subtype = A65_TOKEN_SUBTYPE_UNDEFINED,
			__in_opt int mode = A65_TOKEN_MODE_UNDEFINED
			) const;

		int mode(void) const;

		std::string path(void) const;

		uint16_t scalar(void) const;

		void set(
			__in int type,
			__in_opt int subtype = A65_TOKEN_SUBTYPE_UNDEFINED,
			__in_opt int mode = A65_TOKEN_MODE_UNDEFINED
			);

		void set_literal(
			__in const a65_literal_t &literal
			);

		void set_metadata(
			__in const std::string &path,
			__in size_t line
			);

		void set_scalar(
			__in uint16_t scalar
			);

		int subtype(void) const;

		virtual std::string to_string(void) const;

		int type(void) const;

	protected:

		void decrement(void);

		void generate(void);

		void increment(void);

		a65_uuid_t m_id;

		size_t m_line;

		a65_literal_t m_literal;

		int m_mode;

		std::string m_path;

		uint16_t m_scalar;

		int m_subtype;

		int m_type;
};

#endif // A65_TOKEN_H_
