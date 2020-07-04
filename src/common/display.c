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

#include "./display_type.h"

static char g_title[TITLE_MAX] = {};

sb65_err_t
sb65_display_create(
	__in sb65_display_t *display,
	__in const char *title,
	__in int scale
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!display) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Display=%p", display);
		goto exit;
	}

	if(!title) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Title=%p", title);
		goto exit;
	}

	if((scale < SCALE_MIN) || (scale > SCALE_MAX)) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Scale=%i (expecting %i>=scale<=%i)", scale, SCALE_MIN, SCALE_MAX);
		goto exit;
	}

	display->title = title;

	if(snprintf(g_title, TITLE_MAX, "%s -- %s", display->title, TITLE) < 0) {
		snprintf(g_title, TITLE_MAX, "%s -- %s", UNTITLED, TITLE);
	}

	if((result = sb65_buffer_create(&display->pixel, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(sb65_color_t), 0)) != ERROR_SUCCESS) {
		goto exit_destroy;
	}

	if(!(display->window = SDL_CreateWindow((const char *)g_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			DISPLAY_WIDTH * DISPLAY_SCALE * scale, DISPLAY_HEIGHT * DISPLAY_SCALE * scale, SDL_WINDOW_RESIZABLE))) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_CreateWindow failed: %s", SDL_GetError());
		goto exit_destroy;
	}

	if(!(display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_CreateRenderer failed: %s", SDL_GetError());
		goto exit_destroy;
	}

	if(SDL_RenderSetLogicalSize(display->renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT)) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_RenderSetLogicalSize failed: %s", SDL_GetError());
		goto exit_destroy;
	}

	if(SDL_SetRenderDrawColor(display->renderer, COLOR[COLOR_DEFAULT].red, COLOR[COLOR_DEFAULT].green, COLOR[COLOR_DEFAULT].blue,
			COLOR[COLOR_DEFAULT].alpha)) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_SetRenderDrawColor failed: %s", SDL_GetError());
		goto exit_destroy;
	}

	if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_FALSE) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_SetHint failed: %s", SDL_GetError());
		goto exit_destroy;
	}

	if(!(display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
			DISPLAY_WIDTH, DISPLAY_HEIGHT))) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_CreateTexture failed: %s", SDL_GetError());
		goto exit_destroy;
	}

	result = sb65_display_show(display);

exit_destroy:

	if(result != ERROR_SUCCESS) {
		sb65_display_destroy(display);
	}

exit:
	return result;
}

void
sb65_display_destroy(
	__in sb65_display_t *display
	)
{
	SDL_DestroyTexture(display->texture);
	SDL_DestroyRenderer(display->renderer);
	SDL_DestroyWindow(display->window);
	sb65_buffer_destroy(&display->pixel);
	memset(display, 0, sizeof(*display));
}

#ifndef NDEBUG

void
sb65_display_set_framerate(
	__in sb65_display_t *display,
	__in float rate
	)
{

	if(snprintf(g_title, TITLE_MAX, "%s -- %s [%.01f fps]", display->title, TITLE, rate) < 0) {
		snprintf(g_title, TITLE_MAX, "%s -- %s", UNTITLED, TITLE);
	}

	SDL_SetWindowTitle(display->window, g_title);
}

#endif /* NDEBUG */

void
sb65_display_set_pixel(
	__in sb65_display_t *display,
	__in sb65_col_t color,
	__in uint8_t x,
	__in uint8_t y
	)
{
	((sb65_color_t *)display->pixel.data)[(DISPLAY_WIDTH * y) + x].raw = COLOR[color].raw;
}

sb65_err_t
sb65_display_show(
	__in const sb65_display_t *display
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(SDL_UpdateTexture(display->texture, NULL, (sb65_color_t *)display->pixel.data, DISPLAY_WIDTH * sizeof(sb65_color_t))) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_UpdateTexture failed: %s", SDL_GetError());
		goto exit;
	}

	if(SDL_RenderClear(display->renderer)) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_RenderClear: %s", SDL_GetError());
		goto exit;
	}

	if(SDL_RenderCopy(display->renderer, display->texture, NULL, NULL)) {
		result = SET_ERROR(ERROR_FAILURE, "SDL_RenderCopy failed: %s", SDL_GetError());
		goto exit;
	}

	SDL_RenderPresent(display->renderer);

exit:
	return result;
}
