/**
 * A65
 * Copyright (C) 2018 David Jolly
 *
 * A65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef A65_H_
#define A65_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern int a65_build_archive(int count, const char **input, const char *output, const char *name);

extern int a65_build_object(const char *input, const char *output, int source);

extern int a65_compile(int count, const char **input, const char *output, const char *name);

extern const char *a65_error(void);

extern const char *a65_output_path(void);

extern void a65_version(int *major, int *minor, int *revision);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // A65_H_
