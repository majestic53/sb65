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

#ifndef SB65_COMMON_BUFFER_H_
#define SB65_COMMON_BUFFER_H_

#include "../common.h"

typedef struct {
	uint8_t *data;
	size_t length;
} sb65_buffer_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t sb65_buffer_create(
	__in sb65_buffer_t *buffer,
	__in size_t length,
	__in uint8_t value
	);

void sb65_buffer_destroy(
	__in sb65_buffer_t *buffer
	);

sb65_err_t sb65_buffer_load(
	__in sb65_buffer_t *buffer,
	__in const char *path
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_COMMON_BUFFER_H_ */
