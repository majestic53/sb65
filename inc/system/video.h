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

#ifndef SB65_SYSTEM_VIDEO_H_
#define SB65_SYSTEM_VIDEO_H_

#include "../common/buffer.h"

typedef struct {

	// TODO

} sb65_video_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t sb65_video_create(
	__in sb65_video_t *video,
	__in const sb65_buffer_t *binary,
	__in int scale
	);

void sb65_video_destroy(
	__in sb65_video_t *video
	);

uint8_t sb65_video_read(
	__in const sb65_video_t *video,
	__in uint16_t address
	);

void sb65_video_step(
	__in const sb65_video_t *video
	);

void sb65_video_write(
	__in sb65_video_t *video,
	__in uint16_t address,
	__in uint8_t value
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_SYSTEM_VIDEO_H_ */
