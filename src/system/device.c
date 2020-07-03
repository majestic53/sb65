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
	__in const sb65_buffer_t *binary,
	__in int seed
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!device) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Device=%p", device);
		goto exit;
	}

	srand(device->seed);
	device->random = binary->data[ADDRESS_DEVICE_RANDOM];
	device->input = binary->data[ADDRESS_DEVICE_INPUT];
	device->seed = seed;

	LOG_FORMAT("Device created: Random=%u(%02x), Input=%u(%02x), Seed=%i", device->random, device->random,
		device->input, device->input, device->seed, device->seed);

	if(result != ERROR_SUCCESS) {
		sb65_device_destroy(device);
	}

exit:
	return result;
}

void
sb65_device_destroy(
	__in sb65_device_t *device
	)
{
	LOG("Device destroyed");

	memset(device, 0, sizeof(*device));
}

void
sb65_device_step(
	__in sb65_device_t *device,
	__in uint8_t input
	)
{
	device->random = rand();
	device->input = input;
}

uint8_t
sb65_device_read(
	__in const sb65_device_t *device,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(address) {
		case ADDRESS_DEVICE_RANDOM:
			result = device->random;
			break;
		case ADDRESS_DEVICE_INPUT:
			result = device->input;
			break;
		default:
			result = UINT8_MAX;

			LOG_ERROR_FORMAT("Unsupported read address", "[%04x]->%02x", address, result);
			break;
	}

	return result;
}

void
sb65_device_write(
	__in sb65_device_t *device,
	__in uint16_t address,
	__in uint8_t value
	)
{

	switch(address) {
		default:
			LOG_ERROR_FORMAT("Unsupported write address", "[%04x]<-%02x", address, value);
			break;
	}
}
