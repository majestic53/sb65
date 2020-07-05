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

	processor->p.flag.break_instruction = true;
	processor->p.flag.unused = true;
	processor->sp.word = ADDRESS_STACK_LOW;
	memcpy(processor->iv, &binary->data[ADDRESS_INTERRUPT_LOW], INTERRUPT_LENGTH);
	sb65_processor_interrupt(processor, INTERRUPT_RESET, false);

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
	__in bool set_break
	)
{
	bool taken = false;

	switch(interrupt) {
		case INTERRUPT_NON_MASKABLE:
		case INTERRUPT_RESET:
			taken = true;
			break;
		case INTERRUPT_MASKABLE:
			taken = (!processor->p.flag.interrupt_disable || set_break);
			break;
		default:
			LOG_FORMAT(LEVEL_WARNING, "Unsupported interrupt", "%i", interrupt);
			break;
	}

	if(taken) {
		sb65_runtime_push(&processor->sp.low, processor->pc.low);
		sb65_runtime_push(&processor->sp.low, processor->pc.high);
		sb65_runtime_push(&processor->sp.low, processor->p.low | (set_break ? (1 << FLAG_BREAK_INSTRUCTION) : 0));
		processor->p.flag.interrupt_disable = true;
		processor->pc.word = processor->iv[interrupt].word;
	}
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

bool
sb65_processor_step(
	__in sb65_processor_t *processor
	)
{
	bool result;
	uint32_t cycle;

	// TODO
	cycle = 1;
	// ---

	result = ((processor->cycle += cycle) >= CYCLES_PER_FRAME);
	if(result) {
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
