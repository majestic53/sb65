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

#ifndef SB65_TOOL_LAUNCHER_TYPE_H_
#define SB65_TOOL_LAUNCHER_TYPE_H_

#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/sb65.h"

#define TITLE "SB65"
#define NOTICE "Copyright (C) 2020 David Jolly"
#define USAGE "sb65 [args]"

enum {
	FLAG_HELP = 0,
	FLAG_PATH,
	FLAG_RANDOM,
	FLAG_SCALE,
	FLAG_VERSION,
	FLAG_MAX,
};

#define OPTION_HELP 'h'
#define OPTION_PATH 'p'
#define OPTION_RANDOM 'r'
#define OPTION_SCALE 's'
#define OPTION_VERSION 'v'
#define OPTIONS "hp:r:s:v"
#define OPTIONS_MIN 2

#define SCALE_DEFAULT 1

static const char *FLAG[] = {
	// FLAG_HELP
	"-h",
	// FLAG_PATH
	"-p",
	// FLAG_RANDOM
	"-r",
	// FLAG_SCALE
	"-s",
	// FLAG_VERSION
	"-v",
	};

static const char *FLAG_DESC[] = {
	// FLAG_HELP
	"Display help information",
	// FLAG_PATH
	"Specify binary path",
	// FLAG_RANDOM
	"Specify random seed",
	// FLAG_SCALE
	"Specify display scale",
	// FLAG_VERSION
	"Display version information",
	};

typedef struct {
	sb65_conf_t configuration;
	bool help;
	bool version;
} sb65_launcher_t;

#endif /* SB65_TOOL_LAUNCHER_TYPE_H_ */
