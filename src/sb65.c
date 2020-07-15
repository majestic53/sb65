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

#include "./sb65_type.h"

const char *
sb65_error(void)
{
	return sb65_runtime_error();
}

void
sb65_interrupt(
	__in bool maskable
	)
{
	sb65_runtime_interrupt(maskable ? INTERRUPT_MASKABLE : INTERRUPT_NON_MASKABLE);
}

void
sb65_reset(void)
{
	sb65_runtime_interrupt(INTERRUPT_RESET);
}

int
sb65_setup(
	__in const sb65_conf_t *configuration
	)
{
	return ((sb65_runtime_setup(configuration) != ERROR_SUCCESS) ? EXIT_FAILURE : EXIT_SUCCESS);
}

void
sb65_version(
	__in sb65_ver_t *version
	)
{

	if(version) {
		version->major = VERSION_MAJOR;
		version->minor = VERSION_MINOR;
		version->patch = VERSION_PATCH;
	}
}
