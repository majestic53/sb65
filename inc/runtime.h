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

#ifndef SB65_RUNTIME_H_
#define SB65_RUNTIME_H_

#include "./common.h"
#include "./sb65.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const char *sb65_runtime_error(void);

sb65_err_t sb65_runtime_error_set(
	__in sb65_err_t error,
#ifndef NDEBUG
	__in const char *file,
	__in const char *function,
	__in size_t line,
#endif /* NDEBUG */
	__in const char *format,
	...
	);

void sb65_runtime_interrupt(
	__in sb65_int_t interrupt
	);

#ifndef NDEBUG

void sb65_runtime_log(
	__in FILE *stream,
	__in sb65_lvl_t level,
	__in const char *file,
	__in const char *function,
	__in size_t line,
	__in const char *format,
	...
	);

void sb65_runtime_log_disassemble(
	__in FILE *stream,
	__in uint16_t address,
	__in uint32_t count
	);

void sb65_runtime_log_memory(
	__in FILE *stream,
	__in uint16_t address,
	__in uint32_t offset
	);

void sb65_runtime_log_processor(
	__in FILE *stream
	);

#endif /* NDEBUG */

uint8_t sb65_runtime_read(
	__in uint16_t address
	);

sb65_err_t sb65_runtime_setup(
	__in const sb65_conf_t *configuration
	);

void sb65_runtime_write(
	__in uint16_t address,
	__in uint8_t value
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_RUNTIME_H_ */
