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

#include "./buffer_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t
sb65_buffer_create(
	__in sb65_buffer_t *buffer,
	__in size_t length,
	__in uint8_t value
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!buffer) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Buffer=%p", buffer);
		goto exit;
	}

	if(!length) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Length=%zu", length);
		goto exit;
	}

	buffer->data = (uint8_t *)malloc(length);
	if(!buffer->data) {
		result = SET_ERROR(ERROR_FAILURE, "Data=%p", buffer->data);
		goto exit_destroy;
	}

	buffer->length = length;
	memset(buffer->data, value, buffer->length);

exit_destroy:

	if(result != ERROR_SUCCESS) {
		sb65_buffer_destroy(buffer);
	}

exit:
	return result;
}

void
sb65_buffer_destroy(
	__in sb65_buffer_t *buffer
	)
{

	if(buffer->data) {
		free(buffer->data);
	}

	memset(buffer, 0, sizeof(*buffer));
}

sb65_err_t
sb65_buffer_load(
	__in sb65_buffer_t *buffer,
	__in const char *path
	)
{
	int length;
	FILE *file = NULL;
	sb65_err_t result = ERROR_SUCCESS;

	if(!path) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Path=%p", path);
		goto exit;
	}

	if(!(file = fopen(path, "rb"))) {
		result = SET_ERROR(ERROR_FILE_NOT_FOUND, "Path=%s", path);
		goto exit;
	}

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	if(length > 0) {

		if((result = sb65_buffer_create(buffer, length, 0)) != ERROR_SUCCESS) {
			goto exit;
		}

		length = fread(buffer->data, sizeof(uint8_t), length, file);
	}

	if(length < 0) {
		result = SET_ERROR(ERROR_FAILURE, "Path=%s", path);
		goto exit;
	}

exit:

	if(result != ERROR_SUCCESS) {
		sb65_buffer_destroy(buffer);
	}

	if(file) {
		fclose(file);
		file = NULL;
	}

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
