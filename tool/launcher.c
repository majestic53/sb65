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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static void
sb65_print_version(
	bool verbose
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
	bool verbose
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

static int
sb65_launch(
	int argc,
	char *argv[]
	)
{
	int option, result = EXIT_SUCCESS;

	memset(&g_launcher, 0, sizeof(g_launcher));
	g_launcher.configuration.scale = SCALE_DEFAULT;

	if(argc < OPTIONS_MIN) {
		result = EXIT_FAILURE;
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
				result = EXIT_FAILURE;
				goto exit;
			default:
				result = EXIT_FAILURE;
				goto exit;
		}
	}

	if(g_launcher.help) {
		sb65_print_usage(true);
	} else if(g_launcher.version) {
		sb65_print_version(false);
	} else {

		if((result = sb65_setup(&g_launcher.configuration)) != EXIT_SUCCESS) {
			fprintf(stderr, "Error: %s\n", sb65_error());
			goto exit;
		}
	}

exit:
	return result;
}

int
main(
	int argc,
	char *argv[]
	)
{
	return sb65_launch(argc, argv);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
