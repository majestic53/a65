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

#ifndef A65_ARCHIVE_TYPE_H_
#define A65_ARCHIVE_TYPE_H_

#include "./a65_object_type.h"

#define A65_ARCHIVE_TYPE 2

typedef struct __attribute__((packed)) {
	uint32_t offset;
	uint32_t size;
} a65_archive_object_t;

typedef struct __attribute__((packed)) {
	a65_object_payload_metadata_t metadata;
	a65_archive_object_t object[0];
} a65_archive_payload_t;

#endif // A65_ARCHIVE_TYPE_H_
