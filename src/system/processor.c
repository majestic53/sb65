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

#include "./processor_type.h"

static uint32_t
sb65_processor_execute_brk(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	LOG_FORMAT(LEVEL_INFORMATION, "Processor breakpoint", "%04x", processor->pc.word);

	++processor->pc.word;
	sb65_processor_interrupt(processor, INTERRUPT_MASKABLE, true);

	return 0;
}

static uint32_t
sb65_processor_execute_clc(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.carry = false;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_cld(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.decimal_mode = false;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_cli(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.interrupt_disable = false;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_clv(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.overflow = false;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_dex(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	--processor->x.low;
	processor->sr.flag.negative = BIT_CHECK(processor->x.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->x.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_dey(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	--processor->y.low;
	processor->sr.flag.negative = BIT_CHECK(processor->y.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->y.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_inx(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	++processor->x.low;
	processor->sr.flag.negative = BIT_CHECK(processor->x.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->x.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_iny(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	++processor->y.low;
	processor->sr.flag.negative = BIT_CHECK(processor->y.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->y.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_nop(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->pc.word += INSTRUCTION_LENGTH(instruction->opcode);

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_sec(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.carry = true;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_sed(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.decimal_mode = true;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_sei(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sr.flag.interrupt_disable = true;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_stp(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	LOG_FORMAT(LEVEL_INFORMATION, "Processor entering stop state", "%04x", processor->pc.word);

	processor->stop = true;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_tax(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->x.low = processor->ac.low;
	processor->sr.flag.negative = BIT_CHECK(processor->x.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->x.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_tay(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->y.low = processor->ac.low;
	processor->sr.flag.negative = BIT_CHECK(processor->y.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->y.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_tsx(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->x.low = processor->sp.low;
	processor->sr.flag.negative = BIT_CHECK(processor->x.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->x.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_txa(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->ac.low = processor->x.low;
	processor->sr.flag.negative = BIT_CHECK(processor->ac.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->ac.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_txs(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->sp.low = processor->x.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_tya(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	processor->ac.low = processor->y.low;
	processor->sr.flag.negative = BIT_CHECK(processor->ac.low, BIT_HIGH);
	processor->sr.flag.zero = !processor->ac.low;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static uint32_t
sb65_processor_execute_wai(
	__in sb65_processor_t *processor,
	__in sb65_instruction_t *instruction
	)
{
	LOG_FORMAT(LEVEL_INFORMATION, "Processor entering wait state", "%04x", processor->pc.word);

	processor->wait = true;

	return INSTRUCTION_CYCLE(instruction->opcode).base;
}

static const sb65_instruction_cb EXECUTE[] = {
	// 0x00
	sb65_processor_execute_brk, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x08
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x10
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x18
	sb65_processor_execute_clc, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x20
	NULL, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x28
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x30
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x38
	sb65_processor_execute_sec, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x40
	NULL, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0x48
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x50
	NULL, NULL, NULL, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0x58
	sb65_processor_execute_cli, NULL, NULL, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0x60
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x68
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x70
	NULL, NULL, sb65_processor_execute_nop, NULL,
	NULL, NULL, NULL, NULL,
	// 0x78
	sb65_processor_execute_sei, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x80
	NULL, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x88
	sb65_processor_execute_dey, NULL, sb65_processor_execute_txa, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x90
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x98
	sb65_processor_execute_tya, NULL, sb65_processor_execute_txs, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xa0
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xa8
	sb65_processor_execute_tay, NULL, sb65_processor_execute_tax, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xb0
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xb8
	sb65_processor_execute_clv, NULL, sb65_processor_execute_tsx, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xc0
	NULL, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xc8
	sb65_processor_execute_iny, NULL, sb65_processor_execute_dex, sb65_processor_execute_wai,
	NULL, NULL, NULL, NULL,
	// 0xd0
	NULL, NULL, NULL, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0xd8
	sb65_processor_execute_cld, NULL, NULL, sb65_processor_execute_stp,
	NULL, NULL, NULL, NULL,
	// 0xe0
	NULL, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xe8
	sb65_processor_execute_inx, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0xf0
	NULL, NULL, NULL, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0xf8
	sb65_processor_execute_sed, NULL, NULL, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	};

static uint32_t
sb65_processor_execute(
	__in sb65_processor_t *processor
	)
{
	sb65_instruction_t instruction = {};

	//instruction.opcode = sb65_runtime_read(processor->pc.word++);

// TODO: DEBUG
	instruction.opcode = OPCODE_NOP_IMPLIED;
	processor->pc.word++;
// ---

	return EXECUTE[instruction.opcode](processor, &instruction);
}

static uint32_t
sb65_processor_service(
	__in sb65_processor_t *processor
	)
{
	uint32_t result = 0;

	for(sb65_int_t interrupt = 0; interrupt < INTERRUPT_MAX; ++interrupt) {

		if(processor->iv_state[interrupt].pending) {
			bool breakpoint = processor->iv_state[interrupt].breakpoint, taken = false;

			switch(interrupt) {
				case INTERRUPT_NON_MASKABLE:
				case INTERRUPT_RESET:
					taken = true;
					break;
				case INTERRUPT_MASKABLE:
					taken = !processor->sr.flag.interrupt_disable;
					break;
				default:
					LOG_FORMAT(LEVEL_WARNING, "Unsupported interrupt", "%i", interrupt);
					break;
			}

			if(taken) {
				sb65_processor_push(processor, processor->pc.low);
				sb65_processor_push(processor, processor->pc.high);
				sb65_processor_push(processor, processor->sr.low | (breakpoint ? BIT(FLAG_BREAKPOINT) : 0));
				processor->pc.word = processor->iv[interrupt].word;
				processor->sr.flag.interrupt_disable = true;

				if(breakpoint) {
					processor->sr.flag.decimal_mode = false;
					processor->sr.flag.unused = true;
				}

				if(processor->wait) {
					processor->wait = false;

					LOG_FORMAT(LEVEL_INFORMATION, "Processor leaving wait state", "%04x", processor->pc.word);
				}
			}

			memset(&processor->iv_state[interrupt], 0, sizeof(processor->iv_state[interrupt]));
			result += INTERRUPT_CYCLE;
			break;
		}
	}

	return result;
}

sb65_err_t
sb65_processor_create(
	__in sb65_processor_t *processor,
	__in const sb65_buffer_t *binary
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!processor) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Processor=%p", processor);
		goto exit;
	}

	memcpy(processor->iv, &binary->data[ADDRESS_INTERRUPT_LOW], INTERRUPT_LENGTH);
	processor->sp.word = ADDRESS_STACK_LOW;
	processor->sr.flag.breakpoint = true;
	processor->sr.flag.unused = true;
	sb65_processor_reset(processor);

	LOG_FORMAT(LEVEL_INFORMATION, "Processor created: Non-maskable=%04x, Reset=%04x, Maskable=%04x",
		processor->iv[INTERRUPT_NON_MASKABLE].word, processor->iv[INTERRUPT_RESET].word,
		processor->iv[INTERRUPT_MASKABLE].word);

	if(result != ERROR_SUCCESS) {
		sb65_processor_destroy(processor);
	}

exit:
	return result;
}

void
sb65_processor_destroy(
	__in sb65_processor_t *processor
	)
{
	LOG(LEVEL_INFORMATION, "Processor destroyed");

	memset(processor, 0, sizeof(*processor));
}

void
sb65_processor_interrupt(
	__in sb65_processor_t *processor,
	__in sb65_int_t interrupt,
	__in bool breakpoint
	)
{
	processor->iv_state[interrupt].pending = true;
	processor->iv_state[interrupt].breakpoint = breakpoint;
}

uint8_t
sb65_processor_pull(
	__in sb65_processor_t *processor
	)
{
	return sb65_runtime_read(++processor->sp.low + ADDRESS_STACK_LOW);
}

void
sb65_processor_push(
	__in sb65_processor_t *processor,
	__in uint8_t value
	)
{
	sb65_runtime_write(ADDRESS_STACK_LOW + processor->sp.low--, value);
}

uint8_t
sb65_processor_read(
	__in const sb65_processor_t *processor,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(address) {
		case ADDRESS_INTERRUPT_NON_MASKABLE_HIGH:
			result = processor->iv[INTERRUPT_NON_MASKABLE].high;
			break;
		case ADDRESS_INTERRUPT_NON_MASKABLE_LOW:
			result = processor->iv[INTERRUPT_NON_MASKABLE].low;
			break;
		case ADDRESS_INTERRUPT_RESET_HIGH:
			result = processor->iv[INTERRUPT_RESET].high;
			break;
		case ADDRESS_INTERRUPT_RESET_LOW:
			result = processor->iv[INTERRUPT_RESET].low;
			break;
		case ADDRESS_INTERRUPT_MASKABLE_HIGH:
			result = processor->iv[INTERRUPT_MASKABLE].high;
			break;
		case ADDRESS_INTERRUPT_MASKABLE_LOW:
			result = processor->iv[INTERRUPT_MASKABLE].low;
			break;
		default:
			result = UINT8_MAX;

			LOG_FORMAT(LEVEL_WARNING, "Unsupported read address", "[%04x]->%02x", address, result);
			break;
	}

	return result;
}

void
sb65_processor_reset(
	__in sb65_processor_t *processor
	)
{

	if(processor->stop) {
		processor->stop = false;

		LOG_FORMAT(LEVEL_INFORMATION, "Processor leaving stop state", "%04x", processor->pc.word);
	}

	processor->cycle = 0;
	memset(processor->iv_state, 0, sizeof(*processor->iv_state) * INTERRUPT_MAX);
	sb65_processor_interrupt(processor, INTERRUPT_RESET, false);
}

bool
sb65_processor_step(
	__in sb65_processor_t *processor
	)
{
	bool result;
	uint32_t cycle = 0;

	if(!processor->stop) {
		cycle += sb65_processor_service(processor);

		if(!processor->wait) {
			cycle += sb65_processor_execute(processor);
		} else {
			cycle += INSTRUCTION_CYCLE(OPCODE_NOP_IMPLIED).base;
		}
	} else {
		cycle += INSTRUCTION_CYCLE(OPCODE_NOP_IMPLIED).base;
	}

	if((result = ((processor->cycle += cycle) >= CYCLES_PER_FRAME))) {
		processor->cycle %= CYCLES_PER_FRAME;
	}

	return result;
}

void
sb65_processor_write(
	__in sb65_processor_t *processor,
	__in uint16_t address,
	__in uint8_t value
	)
{

	switch(address) {
		case ADDRESS_INTERRUPT_NON_MASKABLE_HIGH:
			processor->iv[INTERRUPT_NON_MASKABLE].high = value;
			break;
		case ADDRESS_INTERRUPT_NON_MASKABLE_LOW:
			processor->iv[INTERRUPT_NON_MASKABLE].low = value;
			break;
		case ADDRESS_INTERRUPT_RESET_HIGH:
			processor->iv[INTERRUPT_RESET].high = value;
			break;
		case ADDRESS_INTERRUPT_RESET_LOW:
			processor->iv[INTERRUPT_RESET].low = value;
			break;
		case ADDRESS_INTERRUPT_MASKABLE_HIGH:
			processor->iv[INTERRUPT_MASKABLE].high = value;
			break;
		case ADDRESS_INTERRUPT_MASKABLE_LOW:
			processor->iv[INTERRUPT_MASKABLE].low = value;
			break;
		default:
			LOG_FORMAT(LEVEL_WARNING, "Unsupported write address", "[%04x]<-%02x", address, value);
			break;
	}
}
