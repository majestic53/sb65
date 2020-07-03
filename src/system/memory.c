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

#include "./memory_type.h"

sb65_err_t
sb65_memory_create(
	__in sb65_memory_t *memory,
	__in const sb65_buffer_t *binary
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!memory) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Memory=%p", memory);
		goto exit;
	}

	if((result = sb65_buffer_create(&memory->ram, RAM_LENGTH, 0)) != ERROR_SUCCESS) {
		goto exit_destroy;
	}

	if((result = sb65_buffer_create(&memory->stack, STACK_LENGTH, 0)) != ERROR_SUCCESS) {
		goto exit_destroy;
	}

	if((result = sb65_buffer_create(&memory->zero_page, ZERO_PAGE_LENGTH, 0)) != ERROR_SUCCESS) {
		goto exit_destroy;
	}

	memcpy(memory->ram.data, &binary->data[ADDRESS_RAM_LOW], RAM_LENGTH);
	memcpy(memory->stack.data, &binary->data[ADDRESS_STACK_LOW], STACK_LENGTH);
	memcpy(memory->zero_page.data, &binary->data[ADDRESS_ZERO_PAGE_LOW], ZERO_PAGE_LENGTH);

	LOG_FORMAT("Memory created: Ram[%zu]=%p, Stack[%zu]=%p, Zero[%zu]=%p", memory->ram.length, memory->ram.data,
		memory->stack.length, memory->stack.data, memory->zero_page.length, memory->zero_page.data);

exit_destroy:

	if(result != ERROR_SUCCESS) {
		sb65_memory_destroy(memory);
	}

exit:
	return result;
}

void
sb65_memory_destroy(
	__in sb65_memory_t *memory
	)
{
	LOG("Memory destroyed");

	sb65_buffer_destroy(&memory->zero_page);
	sb65_buffer_destroy(&memory->stack);
	sb65_buffer_destroy(&memory->ram);

	memset(memory, 0, sizeof(*memory));
}

uint8_t
sb65_memory_read(
	__in const sb65_memory_t *memory,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(address) {
		case ADDRESS_RAM_LOW ... ADDRESS_RAM_HIGH:
			result = memory->ram.data[address - ADDRESS_RAM_LOW];
			break;
		case ADDRESS_STACK_LOW ... ADDRESS_STACK_HIGH:
			result = memory->stack.data[address - ADDRESS_STACK_LOW];
			break;
		case ADDRESS_ZERO_PAGE_LOW ... ADDRESS_ZERO_PAGE_HIGH:
			result = memory->zero_page.data[address - ADDRESS_ZERO_PAGE_LOW];
			break;
		default:
			result = UINT8_MAX;

			LOG_ERROR_FORMAT("Unsupported read address", "[%04x]->%02x", address, result);
			break;
	}

	return result;
}

void
sb65_memory_write(
	__in sb65_memory_t *memory,
	__in uint16_t address,
	__in uint8_t value
	)
{

	switch(address) {
		case ADDRESS_RAM_LOW ... ADDRESS_RAM_HIGH:
			memory->ram.data[address - ADDRESS_RAM_LOW] = value;
			break;
		case ADDRESS_STACK_LOW ... ADDRESS_STACK_HIGH:
			memory->stack.data[address - ADDRESS_STACK_LOW] = value;
			break;
		case ADDRESS_ZERO_PAGE_LOW ... ADDRESS_ZERO_PAGE_HIGH:
			memory->zero_page.data[address - ADDRESS_ZERO_PAGE_LOW] = value;
			break;
		default:
			LOG_ERROR_FORMAT("Unsupported write address", "[%04x]<-%02x", address, value);
			break;
	}
}
