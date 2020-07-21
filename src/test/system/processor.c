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

#include "../../../inc/common/processor.h"
#include "../../../inc/system/processor.h"

static sb65_err_t g_error = ERROR_SUCCESS;
static uint8_t g_memory[UINT16_MAX + 1] = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t
sb65_runtime_error_set(
	__in sb65_err_t error,
#ifndef NDEBUG
	__in const char *file,
	__in const char *function,
	__in size_t line,
#endif /* NDEBUG */
	__in const char *format,
	...
	)
{
	return (g_error = error);
}

#ifndef NDEBUG

void
sb65_runtime_log(
	__in FILE *stream,
	__in sb65_lvl_t level,
	__in const char *file,
	__in const char *function,
	__in size_t line,
	__in const char *format,
	...
	)
{
	return;
}

#endif /* NDEBUG */

uint8_t
sb65_runtime_read(
	__in uint16_t address
	)
{
	return g_memory[address];
}

uint8_t
sb65_runtime_read_word(
	__in uint16_t address
	)
{
	return (g_memory[address] | (g_memory[address + 1] << CHAR_BIT));
}

void
sb65_runtime_write(
	__in uint16_t address,
	__in uint8_t value
	)
{
	g_memory[address] = value;
}

void
sb65_runtime_write_word(
	__in uint16_t address,
	__in uint8_t value
	)
{
	g_memory[address] = value;
	g_memory[address + 1] = (value >> CHAR_BIT);
}

void
sb65_test_processor_setup(
	__in sb65_processor_t *processor,
	__in const sb65_register_t *address,
	__in const sb65_instruction_t *instruction
	)
{
	g_error = ERROR_SUCCESS;
	memset(g_memory, 0, UINT16_MAX + 1);

	for(size_t index = 0; index < INSTRUCTION_MAX; ++index) {
		g_memory[address->word + index] = instruction->raw[index];
	}

	memset(processor, 0, sizeof(*processor));
	processor->pc.word = address->word;
	processor->sp.low = UINT8_MAX;
}

// TODO

int
main(void)
{
	// TODO
	return EXIT_SUCCESS;
	// ---
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
