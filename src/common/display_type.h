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

#ifndef SB65_COMMON_DISPLAY_TYPE_H_
#define SB65_COMMON_DISPLAY_TYPE_H_

#include "../../inc/common/display.h"
#include "../../inc/runtime.h"

#define COLOR_DEFAULT COLOR_BLACK

#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 6
#define DISPLAY_WIDTH 32

#define SCALE_MIN 1
#define SCALE_MAX 3

#define TITLE "SB65"
#define TITLE_MAX 512

#define UNTITLED "Untitled"

static const sb65_color_t COLOR[] = {
	{{ 0, 0, 0, 255, }},
	{{ 255, 255, 255, 255, }},
	{{ 0, 0, 255, 255, }},
	{{ 255, 255, 0, 255, }},
	{{ 255, 0, 255, 255, }},
	{{ 0, 255, 0, 255, }},
	{{ 255, 0, 0, 255, }},
	{{ 0, 255, 255, 255, }},
	{{ 0, 127, 255, 255, }},
	{{ 0, 60, 90, 255, }},
	{{ 20, 60, 255, 255, }},
	{{ 40, 40, 40, 255, }},
	{{ 100, 100, 100, 255, }},
	{{ 0, 255, 127, 255, }},
	{{ 255, 168, 0, 255, }},
	{{ 160, 160, 160, 255, }},
	};

#endif /* SB65_COMMON_DISPLAY_TYPE_H_ */
