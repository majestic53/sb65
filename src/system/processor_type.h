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

#define CYCLE 1

#define INTERRUPT_CYCLE 7
#define INTERRUPT_LENGTH ADDRESS_LENGTH(ADDRESS_INTERRUPT_LOW, ADDRESS_INTERRUPT_HIGH)

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
	MODE_MAX,
} sb65_mode_t;

typedef enum {

	// TODO
	OPCODE_NOP_IMPLIED = 0xea,
	// ---

} sb65_opcode_t;

typedef struct {
	sb65_mode_t mode;
	uint32_t cycle;
	uint32_t cycle_taken;
} sb65_instruction_t;

static const sb65_instruction_t INSTRUCTION[] = {
	// 0x00
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x08
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x10
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x18
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x20
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x28
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x30
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x38
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x40
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x48
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x50
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x58
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x60
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x68
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x70
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x78
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x80
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x88
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x90
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0x98
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xa0
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xa8
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xb0
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xb8
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xc0
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xc8
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xd0
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xd8
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xe0
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xe8
	{}, {}, { MODE_IMPLIED, 4, 0 }, {}, {}, {}, {}, {},
	// 0xf0
	{}, {}, {}, {}, {}, {}, {}, {},
	// 0xf8
	{}, {}, {}, {}, {}, {}, {}, {},
	};

typedef uint32_t (*sb65_instruction_cb)(
	__in sb65_processor_t *processor,
	__in sb65_opcode_t opcode
	);

#endif /* SB65_SYSTEM_PROCESSOR_TYPE_H_ */
