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

typedef struct {
	const char *path;
	int scale;
	int seed;
} sb65_conf_t;

typedef struct {
	int major;
	int minor;
	int patch;
} sb65_ver_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const char *sb65_error(void);

int sb65_run(const sb65_conf_t *);

void sb65_version(sb65_ver_t *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SB65_H_ */
