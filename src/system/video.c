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

#include "./video_type.h"

sb65_err_t
sb65_video_create(
	__in sb65_video_t *video,
	__in const sb65_buffer_t *binary,
	__in const char *title,
	__in int scale
	)
{
	sb65_err_t result = ERROR_SUCCESS;

	if(!video) {
		result = SET_ERROR(ERROR_INVALID_PARAMETER, "Video=%p", video);
		goto exit;
	}

	if((result = sb65_display_create(&video->display, title, scale)) != ERROR_SUCCESS) {
		goto exit_destroy;
	}

	if((result = sb65_buffer_create(&video->video, VIDEO_LENGTH, 0)) != ERROR_SUCCESS) {
		goto exit_destroy;
	}

	memcpy(video->video.data, &binary->data[ADDRESS_VIDEO_LOW], VIDEO_LENGTH);

	LOG_FORMAT(LEVEL_INFORMATION, "Video created: Title=%s, Scale=%i, Video[%zu]=%p", title, scale,
		video->video.length, video->video.data);

#ifndef NDEBUG
	video->frame_begin = SDL_GetTicks();
	video->frame_end = video->frame_begin;
#endif /* NDEBUG */

exit_destroy:

	if(result != ERROR_SUCCESS) {
		sb65_video_destroy(video);
	}

exit:
	return result;
}

void
sb65_video_destroy(
	__in sb65_video_t *video
	)
{
	LOG(LEVEL_INFORMATION, "Video destroyed");

	sb65_buffer_destroy(&video->video);
	sb65_display_destroy(&video->display);
	memset(video, 0, sizeof(*video));
}

uint8_t
sb65_video_read(
	__in const sb65_video_t *video,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(address) {
		case ADDRESS_VIDEO_LOW ... ADDRESS_VIDEO_HIGH:
			result = video->video.data[address - ADDRESS_VIDEO_LOW];
			break;
		default:
			result = UINT8_MAX;

			LOG_FORMAT(LEVEL_WARNING, "Unsupported read address", "[%04x]->%02x", address, result);
			break;
	}

	return result;
}

sb65_err_t
sb65_video_step(
	__in sb65_video_t *video
	)
{
	++video->frame;

#ifndef NDEBUG
	float framerate;

	if((framerate = (video->frame_end - video->frame_begin)) >= MS_PER_SEC) {
		framerate = (video->frame - ((framerate - MS_PER_SEC) / (float)FRAMES_PER_SECOND));
		sb65_display_set_framerate(&video->display, (framerate > 0.f) ? framerate : 0.f);

		LOG_FORMAT(LEVEL_VERBOSE, "Current framerate: %.01f", (framerate > 0.f) ? framerate : 0.f);

		video->frame_begin = video->frame_end;
		video->frame = 0;
	}

	video->frame_end = SDL_GetTicks();
#endif /* NDEBUG */

	return sb65_display_show(&video->display);
}

void
sb65_video_write(
	__in sb65_video_t *video,
	__in uint16_t address,
	__in uint8_t value
	)
{

	switch(address) {
		case ADDRESS_VIDEO_LOW ... ADDRESS_VIDEO_HIGH:
			video->video.data[address - ADDRESS_VIDEO_LOW] = value;
			break;
		default:
			LOG_FORMAT(LEVEL_WARNING, "Unsupported write address", "[%04x]<-%02x", address, value);
			break;
	}
}
