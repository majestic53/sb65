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

#ifndef SB65_SYSTEM_VIDEO_TYPE_H_
#define SB65_SYSTEM_VIDEO_TYPE_H_

#include "../../inc/system/video.h"
#include "../../inc/runtime.h"
#include "../common/display_type.h"

#define FRAMES_PER_SECOND 60

#define MS_PER_SEC 1000

#define VIDEO_LENGTH ADDRESS_LENGTH(ADDRESS_VIDEO_LOW, ADDRESS_VIDEO_HIGH)

#endif /* SB65_SYSTEM_VIDEO_TYPE_H_ */
