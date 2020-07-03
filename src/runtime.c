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
	sb65_video_destroy(&g_runtime.video);
	sb65_processor_destroy(&g_runtime.processor);
	sb65_memory_destroy(&g_runtime.memory);
	sb65_device_destroy(&g_runtime.device);
	memset(&g_runtime, 0, sizeof(g_runtime));

	LOG("Runtime destroyed");

	SDL_Quit();
}

static sb65_err_t
sb65_runtime_create(
	__in const sb65_conf_t *configuration
	)
{

	sb65_buffer_t binary = {};
	sb65_err_t result = ERROR_SUCCESS;

	if(!configuration->path) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Path=%p", configuration->path);
		goto exit;
	}

	memset(&g_error, 0, sizeof(g_error));

	if(SDL_Init(SDL_INIT_VIDEO)) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_Init failed: %s", SDL_GetError());
		goto exit;
	}

	if((result = sb65_buffer_load(&binary, configuration->path)) != ERROR_SUCCESS) {
		goto exit;
	}

	if(binary.length != BINARY_LENGTH) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Invalid binary: Length=%.01f KB (expecting: %.01f KB)",
					binary.length / BYTE_PER_KBYTE, BINARY_LENGTH / BYTE_PER_KBYTE);
		goto exit;
	}

	LOG_FORMAT("Runtime created: Path=%s, Scale=%u, Seed=%i", configuration->path, configuration->scale, configuration->seed);

	if((result = sb65_device_create(&g_runtime.device, &binary, configuration->seed)) != ERROR_SUCCESS) {
		goto exit;
	}

	if((result = sb65_memory_create(&g_runtime.memory, &binary)) != ERROR_SUCCESS) {
		goto exit;
	}

	if((result = sb65_processor_create(&g_runtime.processor, &binary)) != ERROR_SUCCESS) {
		goto exit;
	}

	if((result = sb65_video_create(&g_runtime.video, &binary, configuration->scale)) != ERROR_SUCCESS) {
		goto exit;
	}

exit:
	sb65_buffer_destroy(&binary);

	if(result != ERROR_SUCCESS) {
		sb65_runtime_destroy();
	}

	return result;
}

static sb65_err_t
sb65_runtime_run(void)
{
	SDL_KeyCode input = 0;
	uint32_t begin = SDL_GetTicks();
	sb65_err_t result = ERROR_SUCCESS;

	LOG("Runtime started");

	for(;;) {
		SDL_Event event = {};
		float framerate, frequency;
		uint32_t cycle = 0, end = SDL_GetTicks();

		if((framerate = (end - begin)) >= MS_PER_SEC) {
			framerate = (g_runtime.frame - ((framerate - MS_PER_SEC) / (float)FRAMES));
#ifndef NDEBUG
			//sb65_display_framerate(&g_runtime.video.display, (framerate > 0.f) ? framerate : 0.f);
#endif /* NDEBUG */
			begin = end;
			g_runtime.frame = 0;
		}

		while(SDL_PollEvent(&event)) {

			switch(event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:

					if(!event.key.repeat) {
						input = SDL_GetKeyFromScancode(event.key.keysym.scancode);

						LOG_FORMAT("Input %s: %u(%02x)", (event.type == SDL_KEYDOWN) ? "pressed" : "released",
								input, input);
					}
					break;
				case SDL_QUIT:
					LOG("Quit event");
					goto exit;
				default:
					break;
			}
		}

		while(cycle++ < CYCLE_PER_FRAME) {
			sb65_device_step(&g_runtime.device, input);
			sb65_processor_step(&g_runtime.processor);
			++g_runtime.cycle;
		}

		sb65_video_step(&g_runtime.video);

		if((frequency = (SDL_GetTicks() - end)) < FRAME_RATE) {
			SDL_Delay(FRAME_RATE - frequency);
		}

		++g_runtime.frame;
	}

exit:
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
		sb65_runtime_destroy();
	}

exit:
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
			snprintf(g_error.message, ERROR_MAX, "%s: %s (%s:%s:%zu)", prefix, message,
					file, function, line);
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
	sb65_processor_interrupt(interrupt);
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

		fprintf(stream, "[%s] {%u (%.02f ms)} %s (%s:%s:%zu)\n", timestamp, g_runtime.cycle, elapsed, message,
			file, function, line);
	} else {
		fprintf(stream, "[%s] {%u (%.02f ms)} (%s:%s:%zu)\n", timestamp, g_runtime.cycle, elapsed,
			file, function,line);
	}
}

#endif /* NDEBUG */

uint8_t
sb65_runtime_pop(
	__in uint16_t *address
	)
{
	return sb65_runtime_read(++(*address));
}

void
sb65_runtime_push(
	__in uint16_t *address,
	__in uint8_t value
	)
{
	sb65_runtime_write((*address)--, value);
}

uint8_t
sb65_runtime_read(
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(address) {
		case ADDRESS_DEVICE_LOW ... ADDRESS_DEVICE_HIGH:
			result = sb65_device_read(&g_runtime.device, address);
			break;
		case ADDRESS_RAM_LOW ... ADDRESS_RAM_HIGH:
		case ADDRESS_STACK_LOW ... ADDRESS_STACK_HIGH:
		case ADDRESS_ZERO_PAGE_LOW ... ADDRESS_ZERO_PAGE_HIGH:
			result = sb65_memory_read(&g_runtime.memory, address);
			break;
		case ADDRESS_VECTOR_LOW ... ADDRESS_VECTOR_HIGH:
			result = sb65_processor_read(&g_runtime.processor, address);
			break;
		case ADDRESS_VIDEO_LOW ... ADDRESS_VIDEO_HIGH:
			result = sb65_video_read(&g_runtime.video, address);
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
			sb65_device_write(&g_runtime.device, address, value);
			break;
		case ADDRESS_RAM_LOW ... ADDRESS_RAM_HIGH:
		case ADDRESS_STACK_LOW ... ADDRESS_STACK_HIGH:
		case ADDRESS_ZERO_PAGE_LOW ... ADDRESS_ZERO_PAGE_HIGH:
			sb65_memory_write(&g_runtime.memory, address, value);
			break;
		case ADDRESS_VECTOR_LOW ... ADDRESS_VECTOR_HIGH:
			sb65_processor_write(&g_runtime.processor, address, value);
			break;
		case ADDRESS_VIDEO_LOW ... ADDRESS_VIDEO_HIGH:
			sb65_video_write(&g_runtime.video, address, value);
			break;
		default:
			LOG_ERROR_FORMAT("Unsupported write address", "[%04x]<-%02x", address, value);
			break;
	}
}
