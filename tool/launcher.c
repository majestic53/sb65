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

#include "./launcher_type.h"

static sb65_launcher_t g_launcher = {};

static void
sb65_print_version(
	__in bool verbose
	)
{
	sb65_ver_t version = {};

	sb65_version(&version);

	if(verbose) {
		fprintf(stdout, "%s ", TITLE);
	}

	fprintf(stdout, "%i.%i.%i\n", version.major, version.minor, version.patch);

	if(verbose) {
		fprintf(stdout, "%s\n", NOTICE);
	}
}

static void
sb65_print_usage(
	__in bool verbose
	)
{

	if(verbose) {
		sb65_print_version(true);
		fprintf(stdout, "\n");
	}

	fprintf(stdout, "%s\n", USAGE);

	for(int flag = 0; flag < FLAG_MAX; ++flag) {

		if(!flag) {
			fprintf(stdout, "\n");
		}

		fprintf(stdout, "%s\t%s\n", FLAG[flag], FLAG_DESC[flag]);
	}
}

static sb65_err_t
sb65_launch(
	__in int argc,
	__in char *argv[]
	)
{
	int option;
	sb65_err_t result = ERROR_SUCCESS;

	memset(&g_launcher, 0, sizeof(g_launcher));
	g_launcher.configuration.scale = SCALE_DEFAULT;

	if(argc < OPTIONS_MIN) {
		result = ERROR_INVALID_PARAMETER;
		sb65_print_usage(true);
		goto exit;
	}

	opterr = 0;

	while((option = getopt(argc, argv, OPTIONS)) != -1) {

		switch(option) {
			case OPTION_HELP:
				g_launcher.help = true;
				break;
			case OPTION_PATH:
				g_launcher.configuration.path = optarg;
				break;
			case OPTION_RANDOM:
				g_launcher.configuration.seed = strtol(optarg, NULL, 10);
				break;
			case OPTION_SCALE:
				g_launcher.configuration.scale = strtol(optarg, NULL, 10);
				break;
			case OPTION_VERSION:
				g_launcher.version = true;
				break;
			case '?':

				switch(optopt) {
					case OPTION_PATH:
						fprintf(stderr, "Option missing: -%c\n", optopt);
						break;
					case OPTION_RANDOM:
					case OPTION_SCALE:
						fprintf(stderr, "Option missing value: -%c\n", optopt);
						break;
					default:

						if(isprint(optopt)) {
							fprintf(stderr, "Unsupported: -%c\n", optopt);
						} else {
							fprintf(stderr, "Unsupported: %i(%x)\n", optopt, optopt);
						}
						break;
				}

				fprintf(stdout, "\n");
				sb65_print_usage(false);
				result = ERROR_INVALID_PARAMETER;
				goto exit;
			default:
				result = ERROR_INVALID_PARAMETER;
				goto exit;
		}
	}

	if(g_launcher.help) {
		sb65_print_usage(true);
	} else if(g_launcher.version) {
		sb65_print_version(false);
	} else {

		if((result = sb65(&g_launcher.configuration)) != ERROR_SUCCESS) {
			fprintf(stderr, "Error: %s\n", sb65_error());
			goto exit;
		}
	}

exit:
	return result;
}

int
main(
	__in int argc,
	__in char *argv[]
	)
{
	return (sb65_launch(argc, argv) == ERROR_SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}
