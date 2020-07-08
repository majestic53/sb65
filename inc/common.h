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

#ifndef SB65_COMMON_H_
#define SB65_COMMON_H_

#include <SDL2/SDL.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef __in
#define __in
#endif /* __in */

#ifndef __out
#define __out
#endif /* __out */

#define ADDRESS_DEVICE_HIGH 0x00ff
#define ADDRESS_DEVICE_LOW 0x00fe
#define ADDRESS_INTERRUPT_HIGH 0xffff
#define ADDRESS_INTERRUPT_LOW 0xfffa
#define ADDRESS_RAM_HIGH 0xfff9
#define ADDRESS_RAM_LOW 0x0600
#define ADDRESS_STACK_HIGH 0x01ff
#define ADDRESS_STACK_LOW 0x0100
#define ADDRESS_VIDEO_HIGH 0x05ff
#define ADDRESS_VIDEO_LOW 0x0200
#define ADDRESS_ZERO_PAGE_HIGH 0x00fd
#define ADDRESS_ZERO_PAGE_LOW 0x0000

#define ADDRESS_DEVICE_RANDOM 0x00fe
#define ADDRESS_DEVICE_INPUT 0x00ff

#define ADDRESS_INTERRUPT_NON_MASKABLE_HIGH 0xfffb
#define ADDRESS_INTERRUPT_NON_MASKABLE_LOW 0xfffa
#define ADDRESS_INTERRUPT_RESET_HIGH 0xfffd
#define ADDRESS_INTERRUPT_RESET_LOW 0xfffc
#define ADDRESS_INTERRUPT_MASKABLE_HIGH 0xffff
#define ADDRESS_INTERRUPT_MASKABLE_LOW 0xfffe

#define CYCLES_PER_FRAME 500

#define FRAMES_PER_SECOND 60

#define MS_PER_SEC 1000

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 6

#define ADDRESS_LENGTH(_LOW_, _HIGH_) \
	(((_HIGH_) + 1) - (_LOW_))

#ifndef NDEBUG
#define LOG(_LEVEL_, _FORMAT_) \
	sb65_runtime_log(((_LEVEL_ == LEVEL_ERROR) || (_LEVEL_ == LEVEL_WARNING)) ? stderr : stdout, _LEVEL_, \
		__FILE__, __FUNCTION__, __LINE__, _FORMAT_, "")
#define LOG_FORMAT(_LEVEL_, _FORMAT_, ...) \
	sb65_runtime_log(((_LEVEL_ == LEVEL_ERROR) || (_LEVEL_ == LEVEL_WARNING)) ? stderr : stdout, _LEVEL_, \
		__FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
#else
#define LOG(_LEVEL_, _FORMAT_)
#define LOG_FORMAT(_LEVEL_, _FORMAT_, ...)
#endif /* NDEBUG */

#ifndef NDEBUG
#define LOG_MEMORY(_STREAM_, _ADDRESS_, _OFFSET_) \
	sb65_runtime_log_memory(_STREAM_, _ADDRESS_, _OFFSET_)
#else
#define LOG_MEMORY(_STREAM_, _ADDRESS_, _OFFSET_)
#endif /* NDEBUG */

#ifndef NDEBUG
#define LOG_PROCESSOR(_STREAM_) \
	sb65_runtime_log_processor(_STREAM_)
#else
#define LOG_PROCESSOR(_STREAM_)
#endif /* NDEBUG */

#ifndef NDEBUG
#define SET_ERROR(_ERROR_, _FORMAT_, ...) \
	sb65_runtime_error_set(_ERROR_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
#else
#define SET_ERROR(_ERROR_, _FORMAT_, ...) \
	sb65_runtime_error_set(_ERROR_, _FORMAT_, __VA_ARGS__)
#endif /* NDEBUG */

typedef enum {
	COLOR_BLACK = 0,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_CYAN,
	COLOR_PURPLE,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_BROWN,
	COLOR_RED_LIGHT,
	COLOR_GREY_DARK,
	COLOR_GREY,
	COLOR_GREEN_LIGHT,
	COLOR_BLUE_LIGHT,
	COLOR_GREY_LIGHT,
	COLOR_MAX,
} sb65_col_t;

typedef enum {
	ERROR_SUCCESS = 0,
	ERROR_FAILURE,
	ERROR_INVALID_PARAMETER,
	ERROR_FILE_NOT_FOUND,
	ERROR_UNKNOWN,
	ERROR_MALFORMED,
} sb65_err_t;

typedef enum {
	INTERRUPT_NON_MASKABLE = 0,
	INTERRUPT_RESET,
	INTERRUPT_MASKABLE,
	INTERRUPT_MAX,
} sb65_int_t;

typedef enum {
	LEVEL_NONE = 0,
	LEVEL_ERROR,
	LEVEL_WARNING,
	LEVEL_INFORMATION,
	LEVEL_VERBOSE,
	LEVEL_MAX,
} sb65_lvl_t;

#endif /* SB65_COMMON_H_ */
