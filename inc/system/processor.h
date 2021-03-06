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

#ifndef SB65_SYSTEM_PROCESSOR_H_
#define SB65_SYSTEM_PROCESSOR_H_

#include "../common/buffer.h"
#include "../common/processor.h"

typedef struct {
	uint32_t cycle;
	sb65_register_t pc;
	sb65_register_t ac;
	sb65_register_t x;
	sb65_register_t y;
	sb65_register_t sr;
	sb65_register_t sp;
	sb65_register_t iv[INTERRUPT_MAX];
	sb65_interrupt_t iv_state[INTERRUPT_MAX];
	bool stop;
	bool wait;
} sb65_processor_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t sb65_processor_create(
	__in sb65_processor_t *processor,
	__in const sb65_buffer_t *binary
	);

void sb65_processor_destroy(
	__in sb65_processor_t *processor
	);

void sb65_processor_interrupt(
	__in sb65_processor_t *processor,
	__in sb65_int_t interrupt,
	__in bool breakpoint
	);

uint8_t sb65_processor_read(
	__in const sb65_processor_t *processor,
	__in uint16_t address
	);

void sb65_processor_reset(
	__in sb65_processor_t *processor
	);

bool sb65_processor_step(
	__in sb65_processor_t *processor
	);

void sb65_processor_write(
	__in sb65_processor_t *processor,
	__in uint16_t address,
	__in uint8_t value
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_SYSTEM_PROCESSOR_H_ */
