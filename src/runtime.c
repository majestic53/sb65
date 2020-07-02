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

#include "./runtime_type.h"

static sb65_error_t g_error = {};

static sb65_runtime_t g_runtime = {};

static void
sb65_runtime_destroy(void)
{
	// TODO: UNLOAD SUBSYSTEMS

	// TODO: UNLOAD SDL

	memset(&g_runtime, 0, sizeof(g_runtime));

	LOG("Runtime destroyed");
}

static sb65_err_t
sb65_runtime_create(
	__in const sb65_conf_t *configuration
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	memset(&g_error, 0, sizeof(g_error));
	g_runtime.configuration = configuration;

	// TODO: LOAD SDL

	// TODO: LOAD SUBSYSTEMS

	LOG_FORMAT("Runtime created: Path=%s, Scale=%u", g_runtime.configuration->path, g_runtime.configuration->scale);

//exit:

	if(result != ERROR_SUCCESS) {
		sb65_runtime_destroy();
	}

	return result;
}

#ifndef NDEBUG

void
sb65_runtime_log(
	__in FILE *stream,
	__in const char *file,
	__in const char *function,
	__in size_t line,
	__in const char *format,
	...
	)
{
	time_t current;
	char timestamp[TIMESTAMP_MAX] = {};
	float elapsed = (g_runtime.cycle * CYCLE_RATE);

	current = time(NULL);
	strftime(timestamp, TIMESTAMP_MAX, TIMESTAMP_FORMAT, localtime(&current));

	if(format) {
		int length = 0;
		va_list arguments;
		char message[TRACE_MAX] = {};

		va_start(arguments, format);
		length = vsnprintf(message, TRACE_MAX, format, arguments);
		va_end(arguments);

		if(length < 0) {
			snprintf(message, TRACE_MAX, "%s", TRACE_MALFORMED);
		}

		fprintf(stream, "[%s] {%lu (%.02f ms)} %s (%s:%s:%zu)\n", timestamp, g_runtime.cycle, elapsed, message, file, function, line);
	} else {
		fprintf(stream, "[%s] {%lu (%.02f ms)} (%s:%s:%zu)\n", timestamp, g_runtime.cycle, elapsed, file, function,line);
	}
}

#endif /* NDEBUG */

static sb65_err_t
sb65_runtime_run(void)
{
	sb65_err_t result = ERROR_SUCCESS;

	LOG("Runtime started");

	// TODO: LOOP PER CYCLE
		// TODO: POLL FOR QUIT
		// TODO: LOOP SUBSYSTEMS

	LOG("Runtime stopped");

	return result;
}

sb65_err_t
sb65_runtime(
	__in const sb65_conf_t *configuration
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!configuration) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Configuration=%p", configuration);
		goto exit;
	}

	if((result = sb65_runtime_create(configuration)) == ERROR_SUCCESS) {
		result = sb65_runtime_run();
	}

exit:
	sb65_runtime_destroy();

	return result;
}

const char *
sb65_runtime_error(void)
{
	return g_error.message;
}

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
	memset(&g_error, 0, sizeof(g_error));

	if((g_error.error = error) != ERROR_SUCCESS) {
		int length = 0;
		va_list arguments;
		char message[ERROR_MAX / 2] = {};

		g_error.error = ((error < ERROR_UNKNOWN) ? error : ERROR_UNKNOWN);
		va_start(arguments, format);
		length = vsnprintf(message, ERROR_MAX / 2, format, arguments);
		va_end(arguments);

		if(length > 0) {
			const char *prefix = ERROR[g_error.error];
#ifndef NDEBUG
			snprintf(g_error.message, ERROR_MAX, "%s: %s (%s:%s:%zu)", prefix, message, file, function, line);
#else
			snprintf(g_error.message, ERROR_MAX, "%s: %s", prefix, message);
#endif /* NDEBUG */
		} else if(length < 0) {
			g_error.error = ERROR_MALFORMED;
			snprintf(g_error.message, ERROR_MAX, "%s", ERROR[ERROR_MALFORMED]);
		}
	}

	return g_error.error;
}

void
sb65_runtime_interrupt(
	__in sb65_int_t interrupt
	)
{
	// TODO: SEND INTERRUPT TO PROCESSOR SUBSYSTEM
}

uint8_t
sb65_runtime_read(
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(address) {
		case ADDRESS_DEVICE_LOW ... ADDRESS_DEVICE_HIGH:

			// TODO: READ FROM DEVICE SUBSYSTEM

			break;
		case ADDRESS_RAM_LOW ... ADDRESS_RAM_HIGH:
		case ADDRESS_STACK_LOW ... ADDRESS_STACK_HIGH:
		case ADDRESS_ZERO_PAGE_LOW ... ADDRESS_ZERO_PAGE_HIGH:

			// TODO: READ FROM MEMORY SUBSYSTEM

			break;
		case ADDRESS_VECTOR_LOW ... ADDRESS_VECTOR_HIGH:

			// TODO: READ FROM PROCESSOR SUBSYSTEM

			break;
		case ADDRESS_VIDEO_LOW ... ADDRESS_VIDEO_HIGH:

			// TODO: READ FROM VIDEO SUBSYSTEM

			break;
		default:
			result = UINT8_MAX;

			LOG_ERROR_FORMAT("Unsupported read address", "[%04x]->%02x", address, result);
			break;
	}

	return result;
}

void
sb65_runtime_write(
	__in uint16_t address,
	__in uint8_t value
	)
{

	switch(address) {
		case ADDRESS_DEVICE_LOW ... ADDRESS_DEVICE_HIGH:

			// TODO: WRITE TO DEVICE SUBSYSTEM

			break;
		case ADDRESS_RAM_LOW ... ADDRESS_RAM_HIGH:
		case ADDRESS_STACK_LOW ... ADDRESS_STACK_HIGH:
		case ADDRESS_ZERO_PAGE_LOW ... ADDRESS_ZERO_PAGE_HIGH:

			// TODO: WRITE TO MEMORY SUBSYSTEM

			break;
		case ADDRESS_VECTOR_LOW ... ADDRESS_VECTOR_HIGH:

			// TODO: WRITE TO PROCESSOR SUBSYSTEM

			break;
		case ADDRESS_VIDEO_LOW ... ADDRESS_VIDEO_HIGH:

			// TODO: WRITE TO VIDEO SUBSYSTEM

			break;
		default:
			LOG_ERROR_FORMAT("Unsupported write address", "[%04x]<-%02x", address, value);
			break;
	}
}
