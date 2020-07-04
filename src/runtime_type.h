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

#ifndef SB65_RUNTIME_TYPE_H_
#define SB65_RUNTIME_TYPE_H_

#include "../inc/system/device.h"
#include "../inc/system/memory.h"
#include "../inc/system/processor.h"
#include "../inc/system/video.h"
#include "../inc/runtime.h"

#define BINARY_LENGTH (UINT16_MAX + 1)

#define BLOCK_WIDTH 16

#define BYTE_PER_KBYTE 1024.f

#define CHARACTER_FILL '.'

#define CYCLE_RATE ((CYCLES_PER_FRAME * FRAMES_PER_SECOND) / 1000000.f) // ~33us/cycle

#define ERROR_MAX 1024

#define FRAME_RATE (MS_PER_SEC / (float)FRAMES_PER_SECOND) // ~16ms/frame

#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define TIMESTAMP_MAX 32

#define TRACE_MALFORMED "Malformed trace"
#define TRACE_MAX 1024

typedef struct {
	sb65_err_t error;
	char message[ERROR_MAX];
} sb65_error_t;

typedef struct {
	uint32_t cycle;
	uint32_t frame;
	sb65_device_t device;
	sb65_memory_t memory;
	sb65_processor_t processor;
	sb65_video_t video;
} sb65_runtime_t;

static const char *ERROR[] = {
	"Success",
	"Failure",
	"Invalid parameter",
	"File not found",
	"Unknown error",
	"Malformed error",
	};

#ifndef NDEBUG

static const char *LEVEL[] = {
	"\x1b[0m",
	"\x1b[91m",
	"\x1b[93m",
	"\x1b[94m",
	"\x1b[90m",
	};

#endif /* NDEBUG */

#endif /* SB65_RUNTIME_TYPE_H_ */
