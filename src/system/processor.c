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
	sb65_register_t indirect = {}, result = {};

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
		case MODE_ZERO_PAGE_RELATIVE:
			result.word = (processor->pc.word + (int8_t)address->low);
			*boundary = (result.high != processor->pc.high);
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
sb65_processor_execute_adc(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_and(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_asl(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_bbr(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(!MASK_CHECK(sb65_runtime_read(instruction->operand.high),
			MASK((instruction->opcode - OPCODE_BBR0_ZERO_PAGE_RELATIVE) / (NIBBLE_MAX + 1)))) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bbs(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(MASK_CHECK(sb65_runtime_read(instruction->operand.high),
			MASK((instruction->opcode - OPCODE_BBS0_ZERO_PAGE_RELATIVE) / (NIBBLE_MAX + 1)))) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bcc(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(!processor->sr.flag.carry) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bcs(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(processor->sr.flag.carry) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_beq(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(processor->sr.flag.zero) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bit(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_bmi(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(processor->sr.flag.negative) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bne(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(!processor->sr.flag.zero) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bpl(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(!processor->sr.flag.negative) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bra(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	bool boundary = false;

	processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);

	return (cycle->base + cycle->branch + (boundary ? cycle->boundary : 0));
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
sb65_processor_execute_bvc(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(!processor->sr.flag.overflow) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
}

static uint32_t
sb65_processor_execute_bvs(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint32_t result = cycle->base;

	if(processor->sr.flag.overflow) {
		bool boundary = false;

		processor->pc.word = sb65_processor_address(processor, &instruction->operand, mode, &boundary);
		result += (cycle->branch + (boundary ? cycle->boundary : 0));
	}

	return result;
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
sb65_processor_execute_cmp(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_cpx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_cpy(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_dec(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_eor(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_inc(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_jmp(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_jsr(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	sb65_processor_push_word(processor, processor->pc.word);
	processor->pc.word = instruction->operand.word;

	return (cycle->base + cycle->read_write_modify);
}

static uint32_t
sb65_processor_execute_lda(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_ldx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_ldy(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_lsr(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_ora(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_rmb(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint8_t value = sb65_runtime_read(instruction->operand.low);

	MASK_CLEAR(value, MASK((instruction->opcode - OPCODE_RMB0_ZERO_PAGE) / (NIBBLE_MAX + 1)));
	sb65_runtime_write(instruction->operand.low, value);

	return (cycle->base + cycle->read_write_modify);
}

static uint32_t
sb65_processor_execute_rol(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_ror(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_sbc(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_smb(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint8_t value = sb65_runtime_read(instruction->operand.low);

	MASK_SET(value, MASK((instruction->opcode - OPCODE_SMB0_ZERO_PAGE) / (NIBBLE_MAX + 1)));
	sb65_runtime_write(instruction->operand.low, value);

	return (cycle->base + cycle->read_write_modify);
}

static uint32_t
sb65_processor_execute_sta(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_stx(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_sty(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
}

static uint32_t
sb65_processor_execute_stz(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	// TODO
	return 0;
	// ---
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
sb65_processor_execute_trb(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint8_t value = sb65_runtime_read(instruction->operand.word);

	processor->sr.flag.zero = (processor->ac.low & value);
	sb65_runtime_write(instruction->operand.word, value & ~processor->ac.low);

	return (cycle->base + cycle->read_write_modify);
}

static uint32_t
sb65_processor_execute_tsb(
	__in sb65_processor_t *processor,
	__in const sb65_instruction_t *instruction,
	__in const sb65_mode_cycle_t *cycle,
	__in sb65_mode_t mode
	)
{
	uint8_t value = sb65_runtime_read(instruction->operand.word);

	processor->sr.flag.zero = (processor->ac.low & value);
	sb65_runtime_write(instruction->operand.word, value | processor->ac.low);

	return (cycle->base + cycle->read_write_modify);
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
	sb65_processor_execute_brk, sb65_processor_execute_ora, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_tsb, sb65_processor_execute_ora, sb65_processor_execute_asl, sb65_processor_execute_rmb,
	// 0x08
	sb65_processor_execute_php, sb65_processor_execute_ora, sb65_processor_execute_asl, sb65_processor_execute_nop,
	sb65_processor_execute_tsb, sb65_processor_execute_ora, sb65_processor_execute_asl, sb65_processor_execute_bbr,
	// 0x10
	sb65_processor_execute_bpl, sb65_processor_execute_ora, sb65_processor_execute_ora, sb65_processor_execute_nop,
	sb65_processor_execute_trb, sb65_processor_execute_ora, sb65_processor_execute_asl, sb65_processor_execute_rmb,
	// 0x18
	sb65_processor_execute_clc, sb65_processor_execute_ora, sb65_processor_execute_inc, sb65_processor_execute_nop,
	sb65_processor_execute_trb, sb65_processor_execute_ora, sb65_processor_execute_asl, sb65_processor_execute_bbr,
	// 0x20
	sb65_processor_execute_jsr, sb65_processor_execute_and, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_bit, sb65_processor_execute_and, sb65_processor_execute_rol, sb65_processor_execute_rmb,
	// 0x28
	sb65_processor_execute_plp, sb65_processor_execute_and, sb65_processor_execute_rol, sb65_processor_execute_nop,
	sb65_processor_execute_bit, sb65_processor_execute_and, sb65_processor_execute_rol, sb65_processor_execute_bbr,
	// 0x30
	sb65_processor_execute_bmi, sb65_processor_execute_and, sb65_processor_execute_and, sb65_processor_execute_nop,
	sb65_processor_execute_bit, sb65_processor_execute_and, sb65_processor_execute_rol, sb65_processor_execute_rmb,
	// 0x38
	sb65_processor_execute_sec, sb65_processor_execute_and, sb65_processor_execute_dec, sb65_processor_execute_nop,
	sb65_processor_execute_bit, sb65_processor_execute_and, sb65_processor_execute_rol, sb65_processor_execute_bbr,
	// 0x40
	sb65_processor_execute_rti, sb65_processor_execute_eor, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_nop, sb65_processor_execute_eor, sb65_processor_execute_lsr, sb65_processor_execute_rmb,
	// 0x48
	sb65_processor_execute_pha, sb65_processor_execute_eor, sb65_processor_execute_lsr, sb65_processor_execute_nop,
	sb65_processor_execute_jmp, sb65_processor_execute_eor, sb65_processor_execute_lsr, sb65_processor_execute_bbr,
	// 0x50
	sb65_processor_execute_bvc, sb65_processor_execute_eor, sb65_processor_execute_eor, sb65_processor_execute_nop,
	sb65_processor_execute_nop, sb65_processor_execute_eor, sb65_processor_execute_lsr, sb65_processor_execute_rmb,
	// 0x58
	sb65_processor_execute_cli, sb65_processor_execute_eor, sb65_processor_execute_phy, sb65_processor_execute_nop,
	sb65_processor_execute_nop, sb65_processor_execute_eor, sb65_processor_execute_lsr, sb65_processor_execute_bbr,
	// 0x60
	sb65_processor_execute_rts, sb65_processor_execute_adc, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_stz, sb65_processor_execute_adc, sb65_processor_execute_ror, sb65_processor_execute_rmb,
	// 0x68
	sb65_processor_execute_pla, sb65_processor_execute_adc, sb65_processor_execute_ror, sb65_processor_execute_nop,
	sb65_processor_execute_jmp, sb65_processor_execute_adc, sb65_processor_execute_ror, sb65_processor_execute_bbr,
	// 0x70
	sb65_processor_execute_bvs, sb65_processor_execute_adc, sb65_processor_execute_adc, sb65_processor_execute_nop,
	sb65_processor_execute_stz, sb65_processor_execute_adc, sb65_processor_execute_ror, sb65_processor_execute_rmb,
	// 0x78
	sb65_processor_execute_sei, sb65_processor_execute_adc, sb65_processor_execute_ply, sb65_processor_execute_nop,
	sb65_processor_execute_jmp, sb65_processor_execute_adc, sb65_processor_execute_ror, sb65_processor_execute_bbr,
	// 0x80
	sb65_processor_execute_bra, sb65_processor_execute_sta, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_sty, sb65_processor_execute_sta, sb65_processor_execute_stx, sb65_processor_execute_smb,
	// 0x88
	sb65_processor_execute_dey, sb65_processor_execute_bit, sb65_processor_execute_txa, sb65_processor_execute_nop,
	sb65_processor_execute_sty, sb65_processor_execute_sta, sb65_processor_execute_stx, sb65_processor_execute_bbs,
	// 0x90
	sb65_processor_execute_bcc, sb65_processor_execute_sta, sb65_processor_execute_sta, sb65_processor_execute_nop,
	sb65_processor_execute_sty, sb65_processor_execute_sta, sb65_processor_execute_stx, sb65_processor_execute_smb,
	// 0x98
	sb65_processor_execute_tya, sb65_processor_execute_sta, sb65_processor_execute_txs, sb65_processor_execute_nop,
	sb65_processor_execute_stz, sb65_processor_execute_sta, sb65_processor_execute_stz, sb65_processor_execute_bbs,
	// 0xa0
	sb65_processor_execute_ldy, sb65_processor_execute_lda, sb65_processor_execute_ldx, sb65_processor_execute_nop,
	sb65_processor_execute_ldy, sb65_processor_execute_lda, sb65_processor_execute_ldx, sb65_processor_execute_smb,
	// 0xa8
	sb65_processor_execute_tay, sb65_processor_execute_lda, sb65_processor_execute_tax, sb65_processor_execute_nop,
	sb65_processor_execute_ldy, sb65_processor_execute_lda, sb65_processor_execute_ldx, sb65_processor_execute_bbs,
	// 0xb0
	sb65_processor_execute_bcs, sb65_processor_execute_lda, sb65_processor_execute_lda, sb65_processor_execute_nop,
	sb65_processor_execute_ldy, sb65_processor_execute_lda, sb65_processor_execute_ldx, sb65_processor_execute_smb,
	// 0xb8
	sb65_processor_execute_clv, sb65_processor_execute_lda, sb65_processor_execute_tsx, sb65_processor_execute_nop,
	sb65_processor_execute_ldy, sb65_processor_execute_lda, sb65_processor_execute_ldx, sb65_processor_execute_bbs,
	// 0xc0
	sb65_processor_execute_cpy, sb65_processor_execute_cmp, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_cpy, sb65_processor_execute_cmp, sb65_processor_execute_dec, sb65_processor_execute_smb,
	// 0xc8
	sb65_processor_execute_iny, sb65_processor_execute_cmp, sb65_processor_execute_dex, sb65_processor_execute_wai,
	sb65_processor_execute_cpy, sb65_processor_execute_cmp, sb65_processor_execute_dec, sb65_processor_execute_bbs,
	// 0xd0
	sb65_processor_execute_bne, sb65_processor_execute_cmp, sb65_processor_execute_cmp, sb65_processor_execute_nop,
	sb65_processor_execute_nop, sb65_processor_execute_cmp, sb65_processor_execute_dec, sb65_processor_execute_smb,
	// 0xd8
	sb65_processor_execute_cld, sb65_processor_execute_cmp, sb65_processor_execute_phx, sb65_processor_execute_stp,
	sb65_processor_execute_nop, sb65_processor_execute_cmp, sb65_processor_execute_dec, sb65_processor_execute_bbs,
	// 0xe0
	sb65_processor_execute_cpx, sb65_processor_execute_smb, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_cpx, sb65_processor_execute_smb, sb65_processor_execute_inc, sb65_processor_execute_smb,
	// 0xe8
	sb65_processor_execute_inx, sb65_processor_execute_sbc, sb65_processor_execute_nop, sb65_processor_execute_nop,
	sb65_processor_execute_cpx, sb65_processor_execute_sbc, sb65_processor_execute_inc, sb65_processor_execute_bbs,
	// 0xf0
	sb65_processor_execute_beq, sb65_processor_execute_sbc, sb65_processor_execute_sbc, sb65_processor_execute_nop,
	sb65_processor_execute_nop, sb65_processor_execute_sbc, sb65_processor_execute_inc, sb65_processor_execute_smb,
	// 0xf8
	sb65_processor_execute_sed, sb65_processor_execute_sbc, sb65_processor_execute_plx, sb65_processor_execute_nop,
	sb65_processor_execute_nop, sb65_processor_execute_sbc, sb65_processor_execute_inc, sb65_processor_execute_bbs,
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
