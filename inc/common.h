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
#define ADDRESS_RAM_HIGH 0xfff9
#define ADDRESS_RAM_LOW 0x0600
#define ADDRESS_STACK_HIGH 0x01ff
#define ADDRESS_STACK_LOW 0x0100
#define ADDRESS_VECTOR_HIGH 0xffff
#define ADDRESS_VECTOR_LOW 0xfffa
#define ADDRESS_VIDEO_HIGH 0x05ff
#define ADDRESS_VIDEO_LOW 0x0200
#define ADDRESS_ZERO_PAGE_HIGH 0x00fd
#define ADDRESS_ZERO_PAGE_LOW 0x0000

#define ADDRESS_DEVICE_RANDOM 0x00fe
#define ADDRESS_DEVICE_INPUT 0x00ff

#define ADDRESS_VECTOR_NON_MASKABLE_HIGH 0xfffb
#define ADDRESS_VECTOR_NON_MASKABLE_LOW 0xfffa
#define ADDRESS_VECTOR_RESET_HIGH 0xfffd
#define ADDRESS_VECTOR_RESET_LOW 0xfffc
#define ADDRESS_VECTOR_MASKABLE_HIGH 0xffff
#define ADDRESS_VECTOR_MASKABLE_LOW 0xfffe

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 2

typedef enum {
	ERROR_SUCCESS = 0,
	ERROR_FAILURE,
	ERROR_INVALID_PARAMETER,
	ERROR_FILE_NOT_FOUND,
	ERROR_UNKNOWN,
	ERROR_MALFORMED,
} sb65_err_t;

typedef enum {
	INTERRUPT_MASKABLE = 0,
	INTERRUPT_NON_MASKABLE,
	INTERRUPT_MAX,
} sb65_int_t;

#define ADDRESS_LENGTH(_LOW_, _HIGH_) \
	(((_HIGH_) + 1) - (_LOW_))

#ifndef NDEBUG
#define LOG(_FORMAT_) \
	sb65_runtime_log(stdout, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, "")
#define LOG_FORMAT(_FORMAT_, ...) \
	sb65_runtime_log(stdout, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
#define LOG_ERROR(_FORMAT_) \
	sb65_runtime_log(stderr, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, "")
#define LOG_ERROR_FORMAT(_FORMAT_, ...) \
	sb65_runtime_log(stderr, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
#else
#define LOG(_FORMAT_)
#define LOG_FORMAT(_FORMAT_, ...)
#define LOG_ERROR(_FORMAT_)
#define LOG_ERROR_FORMAT(_FORMAT_, ...)
#endif /* NDEBUG */

#ifndef NDEBUG
#define SET_ERROR(_ERROR_, _FORMAT_, ...) \
	sb65_runtime_error_set(_ERROR_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
#else
#define SET_ERROR(_ERROR_, _FORMAT_, ...) \
	sb65_runtime_error_set(_ERROR_, _FORMAT_, __VA_ARGS__)
#endif /* NDEBUG */

#endif /* SB65_COMMON_H_ */
