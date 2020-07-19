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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static uint8_t
sb65_processor_pull(
	__in sb65_processor_t *processor
	)
{
	return sb65_runtime_read(++processor->sp.low + ADDRESS_STACK_LOW);
}

static uint16_t
sb65_processor_pull_word(
	__in sb65_processor_t *processor
	)
{
	uint16_t result;

	result = (sb65_runtime_read(++processor->sp.low + ADDRESS_STACK_LOW) << CHAR_BIT);
	result |= sb65_runtime_read(++processor->sp.low + ADDRESS_STACK_LOW);

	return result;
}

static void
sb65_processor_push(
	__in sb65_processor_t *processor,
	__in uint8_t value
	)
{
	sb65_runtime_write(ADDRESS_STACK_LOW + processor->sp.low--, value);
}

static void
sb65_processor_push_word(
	__in sb65_processor_t *processor,
	__in uint16_t value
	)
{
	sb65_runtime_write(ADDRESS_STACK_LOW + processor->sp.low--, value);
	sb65_runtime_write(ADDRESS_STACK_LOW + processor->sp.low--, value >> CHAR_BIT);
}

static uint16_t
sb65_processor_address(
	__in sb65_processor_t *processor,
	__in const sb65_register_t *address,
	__in sb65_mode_t mode,
	__out bool *boundary
	)
{
	sb65_register_t indirect, result = {};

	*boundary = false;

	switch(mode) {
		case MODE_ABSOLUTE:
			result.word = address->word;
			break;
		case MODE_ABSOLUTE_INDIRECT:
			result.word = sb65_runtime_read_word(address->word, false);
			break;
		case MODE_ABSOLUTE_INDIRECT_X:
			result.word = sb65_runtime_read_word(address->word + processor->x.low, false);
			break;
		case MODE_ABSOLUTE_X:
			result.word = (address->word + processor->x.low);
			*boundary = (result.high != address->high);
			break;
		case MODE_ABSOLUTE_Y:
			result.word = (address->word + processor->y.low);
			*boundary = (result.high != address->high);
			break;
		case MODE_RELATIVE:
			result.word = (processor->pc.word + (int8_t)address->low);
			*boundary = (result.high != processor->pc.high);
			break;
		case MODE_ZERO_PAGE:
		case MODE_ZERO_PAGE_RELATIVE:
			result.low = address->low;
			break;
		case MODE_ZERO_PAGE_INDIRECT:
			result.word = sb65_runtime_read_word(address->low, true);
			break;
		case MODE_ZERO_PAGE_INDIRECT_X:
			result.word = sb65_runtime_read_word(address->low + processor->x.low, true);
			break;
		case MODE_ZERO_PAGE_INDIRECT_Y:
			indirect.word = sb65_runtime_read_word(address->low, true);
			result.word = (indirect.word + processor->y.low);
			*boundary = (result.high != indirect.high);
			break;
		case MODE_ZERO_PAGE_X:
			result.low = (address->low + processor->x.low);
			break;
		case MODE_ZERO_PAGE_Y:
			result.low = (address->low + processor->y.low);
			break;
		default:
			LOG_FORMAT(LEVEL_WARNING, "Unsupported mode", "%i", mode);

			break;
	}

	return result.word;
}

static uint32_t
sb65_processor_execute_brk(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	LOG_FORMAT(LEVEL_INFORMATION, "Processor breakpoint", "%04x", processor->pc.word);

	sb65_processor_interrupt(processor, INTERRUPT_MASKABLE, true);

	return 0;
}

static uint32_t
sb65_processor_execute_clc(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.carry = false;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_cld(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.decimal_mode = false;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_cli(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.interrupt_disable = false;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_clv(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.overflow = false;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_dex(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	--processor->x.low;
	processor->sr.flag.negative = MASK_CHECK(processor->x.low, MSB);
	processor->sr.flag.zero = !processor->x.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_dey(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	--processor->y.low;
	processor->sr.flag.negative = MASK_CHECK(processor->y.low, MSB);
	processor->sr.flag.zero = !processor->y.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_inx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	++processor->x.low;
	processor->sr.flag.negative = MASK_CHECK(processor->x.low, MSB);
	processor->sr.flag.zero = !processor->x.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_iny(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	++processor->y.low;
	processor->sr.flag.negative = MASK_CHECK(processor->y.low, MSB);
	processor->sr.flag.zero = !processor->y.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_jsr(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	bool boundary;

	sb65_processor_push_word(processor, processor->pc.word);
	processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);

	return (cycle->base + cycle->read_write_modify);
}

static uint32_t
sb65_processor_execute_nop(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	return cycle->base;
}

static uint32_t
sb65_processor_execute_pha(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	sb65_processor_push(processor, processor->ac.low);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_php(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	sb65_processor_push(processor, processor->sr.low);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_phx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	sb65_processor_push(processor, processor->x.low);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_phy(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	sb65_processor_push(processor, processor->y.low);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_pla(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->ac.low = sb65_processor_pull(processor);
	processor->sr.flag.negative = MASK_CHECK(processor->ac.low, MSB);
	processor->sr.flag.zero = !processor->ac.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_plp(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.low = sb65_processor_pull(processor);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_plx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->x.low = sb65_processor_pull(processor);
	processor->sr.flag.negative = MASK_CHECK(processor->x.low, MSB);
	processor->sr.flag.zero = !processor->x.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_ply(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->y.low = sb65_processor_pull(processor);
	processor->sr.flag.negative = MASK_CHECK(processor->y.low, MSB);
	processor->sr.flag.zero = !processor->y.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_rti(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.low = sb65_processor_pull(processor);
	processor->pc.word = sb65_processor_pull_word(processor);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_rts(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->pc.word = sb65_processor_pull_word(processor);

	return cycle->base;
}

static uint32_t
sb65_processor_execute_sec(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.carry = true;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_sed(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.decimal_mode = true;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_sei(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sr.flag.interrupt_disable = true;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_stp(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	LOG_FORMAT(LEVEL_INFORMATION, "Processor entering stop state", "%04x", processor->pc.word);

	processor->stop = true;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_tax(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->x.low = processor->ac.low;
	processor->sr.flag.negative = MASK_CHECK(processor->x.low, MSB);
	processor->sr.flag.zero = !processor->x.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_tay(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->y.low = processor->ac.low;
	processor->sr.flag.negative = MASK_CHECK(processor->y.low, MSB);
	processor->sr.flag.zero = !processor->y.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_tsx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->x.low = processor->sp.low;
	processor->sr.flag.negative = MASK_CHECK(processor->x.low, MSB);
	processor->sr.flag.zero = !processor->x.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_txa(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->ac.low = processor->x.low;
	processor->sr.flag.negative = MASK_CHECK(processor->ac.low, MSB);
	processor->sr.flag.zero = !processor->ac.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_txs(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->sp.low = processor->x.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_tya(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	processor->ac.low = processor->y.low;
	processor->sr.flag.negative = MASK_CHECK(processor->ac.low, MSB);
	processor->sr.flag.zero = !processor->ac.low;

	return cycle->base;
}

static uint32_t
sb65_processor_execute_wai(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	LOG_FORMAT(LEVEL_INFORMATION, "Processor entering wait state", "%04x", processor->pc.word);

	processor->wait = true;

	return cycle->base;
}

static const sb65_instruction_cb EXECUTE[] = {
	// 0x00
	sb65_processor_execute_brk, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x08
	sb65_processor_execute_php, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x10
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x18
	sb65_processor_execute_clc, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x20
	sb65_processor_execute_jsr, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x28
	sb65_processor_execute_plp, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x30
	NULL, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x38
	sb65_processor_execute_sec, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x40
	sb65_processor_execute_rti, NULL, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0x48
	sb65_processor_execute_pha, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x50
	NULL, NULL, NULL, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0x58
	sb65_processor_execute_cli, NULL, sb65_processor_execute_phy, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	// 0x60
	sb65_processor_execute_rts, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x68
	sb65_processor_execute_pla, NULL, NULL, sb65_processor_execute_nop,
	NULL, NULL, NULL, NULL,
	// 0x70
	NULL, NULL, sb65_processor_execute_nop, NULL,
	NULL, NULL, NULL, NULL,
	// 0x78
	sb65_processor_execute_sei, NULL, sb65_processor_execute_ply, sb65_processor_execute_nop,
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
	sb65_processor_execute_cld, NULL, sb65_processor_execute_phx, sb65_processor_execute_stp,
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
	sb65_processor_execute_sed, NULL, sb65_processor_execute_plx, sb65_processor_execute_nop,
	sb65_processor_execute_nop, NULL, NULL, NULL,
	};

static uint32_t
sb65_processor_execute(
	__in sb65_processor_t *processor
	)
{
	uint32_t length;
	sb65_instruction_t instruction = {};

	instruction.opcode = sb65_runtime_read(processor->pc.word++);

	switch((length = INSTRUCTION_LENGTH(instruction.opcode))) {
		case LENGTH_WORD:
			instruction.operand.word = sb65_runtime_read_word(processor->pc.word, false);
			break;
		case LENGTH_BYTE:
			instruction.operand.low = sb65_runtime_read(processor->pc.word);
			break;
		default:
			break;
	}

	processor->pc.word += length;

	return EXECUTE[instruction.opcode](processor, &instruction, &INSTRUCTION_CYCLE(instruction.opcode),
		INSTRUCTION_MODE(instruction.opcode));
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
				sb65_processor_push_word(processor, processor->pc.word);
				sb65_processor_push(processor, processor->sr.low | (breakpoint ? MASK(FLAG_BREAKPOINT) : 0));
				processor->pc.word = processor->iv[interrupt].word;
				processor->sr.flag.interrupt_disable = true;
				processor->sr.flag.decimal_mode = false;
				processor->sr.flag.unused = true;

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

#ifdef __cplusplus
}
#endif /* __cplusplus */
