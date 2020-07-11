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
#include "./system/processor_type.h"
#include "./system/video_type.h"

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

static const char *MODE_FORMAT[] = {
	"$%04x",
	"($%04x, x)",
	"$%04x, x",
	"$%04x, y",
	"($%04x)",
	"A",
	"#$%02x",
	"",
	"$%02x(%i) [%04x]",
	"",
	"$%02x",
	"($%02x, x)",
	"$%02x, x",
	"$%02x, y",
	"($%02x), y",
	"($%02x)",
	};

static const char *OPCODE_FORMAT[] = {
	// 0x00
	"brk", "ora", "???", "???", "tsb", "ora", "asl", "rmb0",

	// 0x08
	"php", "ora", "asl", "???", "tsb", "ora", "asl", "bbr0",

	// 0x10
	"bpl", "ora", "ora", "???", "trb", "ora", "asl", "rmb1",

	// 0x18
	"clc", "ora", "inc", "???", "trb", "ora", "asl", "bbr1",

	// 0x20
	"jsr", "and", "???", "???", "bit", "and", "rol", "rmb2",

	// 0x28
	"plp", "and", "rol", "???", "bit", "and", "rol", "bbr2",

	// 0x30
	"bmi", "and", "and", "???", "bit", "and", "rol", "rmb3",

	// 0x38
	"sec", "and", "dec", "???", "bit", "and", "rol", "bbr3",

	// 0x40
	"rti", "eor", "???", "???", "???", "eor", "lsr", "rmb4",

	// 0x48
	"pha", "eor", "lsr", "???", "jmp", "eor", "lsr", "bbr4",

	// 0x50
	"bvc", "eor", "eor", "???", "???", "eor", "lsr", "rmb5",

	// 0x58
	"cli", "eor", "phy", "???", "???", "eor", "lsr", "bbr5",

	// 0x60
	"rts", "adc", "???", "???", "stz", "adc", "ror", "rmb6",

	// 0x68
	"pla", "adc", "ror", "???", "jmp", "adc", "ror", "bbr6",

	// 0x70
	"bvs", "adc", "adc", "???", "stz", "adc", "ror", "rmb7",

	// 0x78
	"sei", "adc", "ply", "???", "jmp", "adc", "ror", "bbr7",

	// 0x80
	"bra", "sta", "???", "???", "sty", "sta", "stx", "smb0",

	// 0x88
	"dey", "bit", "txa", "???", "sty", "sta", "stx", "bbs0",

	// 0x90
	"bcc", "sta", "sta", "???", "sty", "sta", "stx", "smb1",

	// 0x98
	"tya", "sta", "txs", "???", "stz", "sta", "stz", "bbs1",

	// 0xa0
	"ldy", "lda", "ldx", "???", "ldy", "lda", "ldx", "smb2",

	// 0xa8
	"tay", "lda", "tax", "???", "ldy", "lda", "ldx", "bbs2",

	// 0xb0
	"bcs", "lda", "lda", "???", "ldy", "lda", "ldx", "smb3",

	// 0xb8
	"clv", "lda", "tsx", "???", "ldy", "lda", "ldx", "bbs3",

	// 0xc0
	"cpy", "cmp", "???", "???", "cpy", "cmp", "dec", "smb4",

	// 0xc8
	"iny", "cmp", "dex", "wai", "cpy", "cmp", "dec", "bbs4",

	// 0xd0
	"bne", "cmp", "cmp", "???", "???", "cmp", "dec", "smb5",

	// 0xd8
	"cld", "cmp", "phx", "stp", "???", "cmp", "dec", "bbs5",

	// 0xe0
	"cpx", "sbc", "???", "???", "cpx", "sbc", "inc", "smb6",

	// 0xe8
	"inx", "sbc", "nop", "???", "cpx", "sbc", "inc", "bbs6",

	// 0xf0
	"beq", "sbc", "sbc", "???", "???", "sbc", "inc", "smb7",

	// 0xf8
	"sed", "sbc", "plx", "???", "???", "sbc", "inc", "bbs7",
};

#endif /* NDEBUG */

#endif /* SB65_RUNTIME_TYPE_H_ */
