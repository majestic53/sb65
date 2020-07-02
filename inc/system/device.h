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

#ifndef SB65_SYSTEM_DEVICE_H_
#define SB65_SYSTEM_DEVICE_H_

#include "../common/buffer.h"

typedef struct {
	uint64_t cycle;
	uint8_t random;
	uint8_t input;
} sb65_device_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t sb65_device_create(
	__in sb65_device_t *device,
	__in const sb65_buffer_t *binary
	);

void sb65_device_destroy(
	__in sb65_device_t *device
	);

// TODO

uint8_t sb65_device_read(
	__in const sb65_device_t *device,
	__in uint16_t address
	);

void sb65_device_write(
	__in sb65_device_t *device,
	__in uint16_t address,
	__in uint8_t value
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_SYSTEM_DEVICE_H_ */
