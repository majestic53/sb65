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

#ifndef SB65_H_
#define SB65_H_

/****************************
 * SB65 types
 ****************************/

/**
 * Configuration struct
 */
typedef struct {
	/* Binary path */
	const char *path;
	/* Display scale */
	int scale;
	/* Device seed */
	int seed;
} sb65_conf_t;

/**
 * Version struct
 */
typedef struct {
	/* Major version */
	int major;
	/* Minor version */
	int minor;
	/* Patch version */
	int patch;
} sb65_ver_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************
 * SB65 runtime routines
 ****************************/

/**
 * Setup sandbox environment
 * @param Const pointer to configuration struct
 * @return EXIT_SUCCESS on success
 */
int sb65_setup(const sb65_conf_t *);

/**
 * Reset sandbox environment
 */
void sb65_reset(void);

/**
 * Interrupt sandbox environment
 * @param Maskable interrupt
 */
void sb65_interrupt(bool);

/****************************
 * SB65 misc. routines
 ****************************/

/**
 * Retrieve error string
 * @return Const pointer to error string
 */
const char *sb65_error(void);

/**
 * Retrieve version
 * @param Pointer to version struct
 */
void sb65_version(sb65_ver_t *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_H_ */
