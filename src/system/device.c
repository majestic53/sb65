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

#include "./device_type.h"

sb65_err_t
sb65_device_create(
	__in sb65_device_t *device,
	__in const sb65_buffer_t *binary
	)
{
	// TOOD
	return ERROR_SUCCESS;
	// ---
}

void
sb65_device_destroy(
	__in sb65_device_t *device
	)
{
	// TODO
}

uint8_t
sb65_device_read(
	__in const sb65_device_t *device,
	__in uint16_t address
	)
{
	// TODO
	return 0;
	// ---
}

void
sb65_device_write(
	__in sb65_device_t *device,
	__in uint16_t address,
	__in uint8_t value
	)
{
	// TODO
}
