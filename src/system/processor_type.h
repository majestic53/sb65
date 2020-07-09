/**
 * SB65
 * Copyright (C) 2020 David Jolly
 *
 * SB65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SB65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SB65_SYSTEM_PROCESSOR_TYPE_H_
#define SB65_SYSTEM_PROCESSOR_TYPE_H_

#include "../../inc/system/processor.h"
#include "../../inc/runtime.h"

#define ADDRESS_INTERRUPT_NON_MASKABLE_HIGH 0xfffb
#define ADDRESS_INTERRUPT_NON_MASKABLE_LOW 0xfffa
#define ADDRESS_INTERRUPT_RESET_HIGH 0xfffd
#define ADDRESS_INTERRUPT_RESET_LOW 0xfffc
#define ADDRESS_INTERRUPT_MASKABLE_HIGH 0xffff
#define ADDRESS_INTERRUPT_MASKABLE_LOW 0xfffe

#define CYCLES_PER_FRAME 500

#define INTERRUPT_CYCLE 7
#define INTERRUPT_LENGTH ADDRESS_LENGTH(ADDRESS_INTERRUPT_LOW, ADDRESS_INTERRUPT_HIGH)

#define INSTRUCTION_CYCLE(_OPCODE_) \
	MODE_CYCLE[MODE[_OPCODE_]]

#define INSTRUCTION_LENGTH(_OPCODE_) \
	MODE_LENGTH[MODE[_OPCODE_]]

typedef enum {
	FLAG_CARRY = 0,
	FLAG_ZERO,
	FLAG_INTERRUPT_DISABLE,
	FLAG_DECIMAL_NODE,
	FLAG_BREAKPOINT,
	FLAG_UNUSED,
	FLAG_OVERFLOW,
	FLAG_NEGATIVE,
	FLAG_MAX,
} sb65_flag_t;

typedef enum {
	MODE_ABSOLUTE = 0,
	MODE_ABSOLUTE_INDIRECT_X,
	MODE_ABSOLUTE_X,
	MODE_ABSOLUTE_Y,
	MODE_ABSOLUTE_INDIRECT,
	MODE_ACCUMULATOR,
	MODE_IMMEDIATE,
	MODE_IMPLIED,
	MODE_RELATIVE,
	MODE_STACK,
	MODE_ZERO_PAGE,
	MODE_ZERO_PAGE_INDIRECT_X,
	MODE_ZERO_PAGE_X,
	MODE_ZERO_PAGE_Y,
	MODE_ZERO_PAGE_INDIRECT_Y,
	MODE_ZERO_PAGE_INDIRECT,
	MODE_MAX,
} sb65_mode_t;

typedef enum {

	// TODO
	OPCODE_NOP_IMPLIED = 0xea,
	// ---

} sb65_opcode_t;

static const sb65_mode_t MODE[] = {
	// 0x00
	MODE_STACK, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0x08
	MODE_STACK, MODE_IMMEDIATE, MODE_ABSOLUTE, MODE_IMPLIED, MODE_ACCUMULATOR, MODE_ACCUMULATOR, MODE_ACCUMULATOR, MODE_RELATIVE,
	// 0x10
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE,
	// 0x18
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_ACCUMULATOR, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	// 0x20
	MODE_ABSOLUTE, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0x28
	MODE_STACK, MODE_IMMEDIATE, MODE_ACCUMULATOR, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0x30
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE,
	// 0x38
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_ACCUMULATOR, MODE_IMPLIED, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	// 0x40
	MODE_STACK, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0x48
	MODE_STACK, MODE_IMMEDIATE, MODE_ACCUMULATOR, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0x50
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE,
	// 0x58
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_STACK, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	// 0x60
	MODE_STACK, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0x68
	MODE_STACK, MODE_IMMEDIATE, MODE_ACCUMULATOR, MODE_IMPLIED, MODE_ABSOLUTE_INDIRECT, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0x70
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE,
	// 0x78
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_STACK, MODE_IMPLIED, MODE_ABSOLUTE_INDIRECT_X, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	// 0x80
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0x88
	MODE_IMPLIED, MODE_IMMEDIATE, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0x90
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_Y, MODE_ZERO_PAGE,
	// 0x98
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	// 0xa0
	MODE_IMMEDIATE, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMMEDIATE, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0xa8
	MODE_IMPLIED, MODE_IMMEDIATE, MODE_IMPLIED, MODE_IMPLIED, MODE_ACCUMULATOR, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0xb0
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_Y, MODE_ZERO_PAGE,
	// 0xb8
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_ABSOLUTE_Y, MODE_RELATIVE,
	// 0xc0
	MODE_IMMEDIATE, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0xc8
	MODE_IMPLIED, MODE_IMMEDIATE, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0xd0
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE,
	// 0xd8
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_STACK, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	// 0xe0
	MODE_IMMEDIATE, MODE_ZERO_PAGE_INDIRECT_X, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE, MODE_ZERO_PAGE,
	// 0xe8
	MODE_IMPLIED, MODE_IMMEDIATE, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_ABSOLUTE, MODE_RELATIVE,
	// 0xf0
	MODE_RELATIVE, MODE_ZERO_PAGE_INDIRECT_Y, MODE_ZERO_PAGE_INDIRECT, MODE_IMPLIED, MODE_IMPLIED, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE_X, MODE_ZERO_PAGE,
	// 0xf8
	MODE_IMPLIED, MODE_ABSOLUTE_Y, MODE_STACK, MODE_IMPLIED, MODE_IMPLIED, MODE_ABSOLUTE_X, MODE_ABSOLUTE_X, MODE_RELATIVE,
	};

typedef struct {
	uint32_t base;
	uint32_t boundary;
	uint32_t branch;
	uint32_t read_write_modify;
} sb65_mode_cycle_t;

static const sb65_mode_cycle_t MODE_CYCLE[] = {
	{ 4, 0, 0, 2 }, { 6, 0, 0, 0 }, { 4, 1, 0, 2 }, { 4, 1, 0, 0 },
	{ 6, 0, 0, 0 }, { 2, 0, 0, 0 }, { 2, 0, 0, 0 }, { 2, 0, 0, 0 },
	{ 2, 1, 1, 0 }, { 3, 0, 0, 0 }, { 3, 0, 0, 2 }, { 6, 0, 0, 0 },
	{ 4, 0, 0, 2 }, { 4, 0, 0, 0 }, { 5, 0, 0, 0 }, { 5, 1, 0, 0 },
	};

static const size_t MODE_LENGTH[] = {
	3, 3, 3, 3, 3, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2,
	};

typedef uint32_t (*sb65_instruction_cb)(
	__in sb65_processor_t *processor,
	__in sb65_opcode_t opcode
	);

#endif /* SB65_SYSTEM_PROCESSOR_TYPE_H_ */
