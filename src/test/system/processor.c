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

#include <check.h>
#include "../../../inc/system/processor.h"

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
	// TODO
	return error;
	// ---
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
	// TODO
}

#endif /* NDEBUG */

uint8_t
sb65_runtime_read(
	__in uint16_t address
	)
{
	// TODO
	return 0;
	// ---
}

void
sb65_runtime_write(
	__in uint16_t address,
	__in uint8_t value
	)
{
	// TODO
}

// TODO

int
main(void)
{
	return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
