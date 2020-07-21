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

#define MSB MASK(CHAR_BIT - 1)
#define LSB MASK(0)

#define MASK(_BIT_) \
	(1 << (_BIT_))

#define MASK_CHECK(_VALUE_, _MASK_) \
	(((_VALUE_) & (_MASK_)) == (_MASK_))

#define MASK_CLEAR(_VALUE_, _MASK_) \
	((_VALUE_) &= ~(_MASK_))

#define MASK_SET(_VALUE_, _MASK_) \
	((_VALUE_) |= (_MASK_))

#define CYCLES_PER_FRAME 500

#define INTERRUPT_CYCLE 7
#define INTERRUPT_LENGTH ADDRESS_LENGTH(ADDRESS_INTERRUPT_LOW, ADDRESS_INTERRUPT_HIGH)

#define INSTRUCTION_CYCLE(_OPCODE_) \
	MODE_CYCLE[INSTRUCTION_MODE(_OPCODE_)]

#define INSTRUCTION_LENGTH(_OPCODE_) \
	MODE_LENGTH[INSTRUCTION_MODE(_OPCODE_)]

#define INSTRUCTION_MODE(_OPCODE_) \
	MODE[_OPCODE_]

#define NIBBLE_MAX 0x0f

typedef uint32_t (*sb65_instruction_cb)(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	);

#endif /* SB65_SYSTEM_PROCESSOR_TYPE_H_ */
