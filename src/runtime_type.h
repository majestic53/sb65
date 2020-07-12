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
	// ERROR_SUCCESS
	"Success",
	// ERROR_FAILURE
	"Failure",
	// ERROR_INVALID_PARAMETER
	"Invalid parameter",
	// ERROR_FILE_NOT_FOUND
	"File not found",
	// ERROR_UNKNOWN
	"Unknown error",
	// ERROR_MALFORMED
	"Malformed error",
	};

#ifndef NDEBUG

static const char *LEVEL[] = {
	// LEVEL_NONE
	"\x1b[0m",
	// LEVEL_ERROR
	"\x1b[91m",
	// LEVEL_WARNING
	"\x1b[93m",
	// LEVEL_INFORMATION
	"\x1b[94m",
	// LEVEL_VERBOSE
	"\x1b[90m",
	};

#define INSTRUCTION_FORMAT(_OPCODE_) \
	MODE_FORMAT[MODE[_OPCODE_]]

static const char *MODE_FORMAT[] = {
	// MODE_ABSOLUTE
	"%04x",
	// MODE_ABSOLUTE_INDIRECT
	"(%04x)",
	// MODE_ABSOLUTE_INDIRECT_X
	"(%04x, x)",
	// MODE_ABSOLUTE_X
	"%04x, x",
	// MODE_ABSOLUTE_Y
	"%04x, y",
	// MODE_ACCUMULATOR
	"a",
	// MODE_IMMEDIATE
	"#%02x",
	// MODE_IMPLIED
	"",
	// MODE_RELATIVE
	"%02x(%i) [%04x]",
	// MODE_STACK
	"",
	// MODE_UNUSED
	"",
	// MODE_ZERO_PAGE
	"%02x",
	// MODE_ZERO_PAGE_INDIRECT
	"(%02x)",
	// MODE_ZERO_PAGE_INDIRECT_X
	"(%02x, x)",
	// MODE_ZERO_PAGE_INDIRECT_Y
	"(%02x), y",
	// MODE_ZERO_PAGE_RELATIVE
	"%02x, %02x(%i) [%04x]",
	// MODE_ZERO_PAGE_X
	"%02x, x",
	// MODE_ZERO_PAGE_Y
	"%02x, y",
	};

static const char *OPCODE_FORMAT[] = {
	// 0x00
	"brk", "ora", "nop", "nop", "tsb", "ora", "asl", "rmb0",
	// 0x08
	"php", "ora", "asl", "nop", "tsb", "ora", "asl", "bbr0",
	// 0x10
	"bpl", "ora", "ora", "nop", "trb", "ora", "asl", "rmb1",
	// 0x18
	"clc", "ora", "inc", "nop", "trb", "ora", "asl", "bbr1",
	// 0x20
	"jsr", "and", "nop", "nop", "bit", "and", "rol", "rmb2",
	// 0x28
	"plp", "and", "rol", "nop", "bit", "and", "rol", "bbr2",
	// 0x30
	"bmi", "and", "and", "nop", "bit", "and", "rol", "rmb3",
	// 0x38
	"sec", "and", "dec", "nop", "bit", "and", "rol", "bbr3",
	// 0x40
	"rti", "eor", "nop", "nop", "nop", "eor", "lsr", "rmb4",
	// 0x48
	"pha", "eor", "lsr", "nop", "jmp", "eor", "lsr", "bbr4",
	// 0x50
	"bvc", "eor", "eor", "nop", "nop", "eor", "lsr", "rmb5",
	// 0x58
	"cli", "eor", "phy", "nop", "nop", "eor", "lsr", "bbr5",
	// 0x60
	"rts", "adc", "nop", "nop", "stz", "adc", "ror", "rmb6",
	// 0x68
	"pla", "adc", "ror", "nop", "jmp", "adc", "ror", "bbr6",
	// 0x70
	"bvs", "adc", "adc", "nop", "stz", "adc", "ror", "rmb7",
	// 0x78
	"sei", "adc", "ply", "nop", "jmp", "adc", "ror", "bbr7",
	// 0x80
	"bra", "sta", "nop", "nop", "sty", "sta", "stx", "smb0",
	// 0x88
	"dey", "bit", "txa", "nop", "sty", "sta", "stx", "bbs0",
	// 0x90
	"bcc", "sta", "sta", "nop", "sty", "sta", "stx", "smb1",
	// 0x98
	"tya", "sta", "txs", "nop", "stz", "sta", "stz", "bbs1",
	// 0xa0
	"ldy", "lda", "ldx", "nop", "ldy", "lda", "ldx", "smb2",
	// 0xa8
	"tay", "lda", "tax", "nop", "ldy", "lda", "ldx", "bbs2",
	// 0xb0
	"bcs", "lda", "lda", "nop", "ldy", "lda", "ldx", "smb3",
	// 0xb8
	"clv", "lda", "tsx", "nop", "ldy", "lda", "ldx", "bbs3",
	// 0xc0
	"cpy", "cmp", "nop", "nop", "cpy", "cmp", "dec", "smb4",
	// 0xc8
	"iny", "cmp", "dex", "wai", "cpy", "cmp", "dec", "bbs4",
	// 0xd0
	"bne", "cmp", "cmp", "nop", "nop", "cmp", "dec", "smb5",
	// 0xd8
	"cld", "cmp", "phx", "stp", "nop", "cmp", "dec", "bbs5",
	// 0xe0
	"cpx", "sbc", "nop", "nop", "cpx", "sbc", "inc", "smb6",
	// 0xe8
	"inx", "sbc", "nop", "nop", "cpx", "sbc", "inc", "bbs6",
	// 0xf0
	"beq", "sbc", "sbc", "nop", "nop", "sbc", "inc", "smb7",
	// 0xf8
	"sed", "sbc", "plx", "nop", "nop", "sbc", "inc", "bbs7",
};

#endif /* NDEBUG */

#endif /* SB65_RUNTIME_TYPE_H_ */
