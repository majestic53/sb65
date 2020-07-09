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

#ifndef SB65_COMMON_DISPLAY_H_
#define SB65_COMMON_DISPLAY_H_

#include "./buffer.h"

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

typedef union {

	struct {
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	};

	uint32_t raw;
} sb65_color_t;

typedef struct {
	sb65_buffer_t pixel;
	SDL_Texture *texture;
	const char *title;
	SDL_Renderer *renderer;
	SDL_Window *window;
} sb65_display_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sb65_err_t sb65_display_create(
	__in sb65_display_t *display,
	__in const char *title,
	__in int scale
	);

void sb65_display_destroy(
	__in sb65_display_t *display
	);

#ifndef NDEBUG

void sb65_display_set_framerate(
	__in sb65_display_t *display,
	__in float rate
	);

#endif /* NDEBUG */

void sb65_display_set_pixel(
	__in sb65_display_t *display,
	__in sb65_col_t color,
	__in uint8_t x,
	__in uint8_t y
	);

sb65_err_t sb65_display_show(
	__in const sb65_display_t *display
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_COMMON_DISPLAY_H_ */
