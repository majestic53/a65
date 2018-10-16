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

#ifndef A65_OBJECT_TYPE_H_
#define A65_OBJECT_TYPE_H_

#include "./a65_define.h"

#define A65_OBJECT_MAGIC 0x353641

#define A65_OBJECT_SECTION_NAME_MAX 24

#define A65_OBJECT_TYPE 1

typedef struct __attribute__((packed)) {
	uint16_t major : 4;
	uint16_t minor : 4;
	uint16_t type : 4;
	uint16_t reserved : 4;
} a65_object_header_metadata_t;

typedef struct __attribute__((packed)) {
	uint32_t magic;
	a65_object_header_metadata_t metadata;
} a65_object_header_t;

typedef struct __attribute__((packed)) {
	uint16_t origin;
	uint32_t offset;
	uint32_t size;
	char name[A65_OBJECT_SECTION_NAME_MAX];
} a65_object_section_t;

typedef struct __attribute__((packed)) {
	uint32_t count;
	uint32_t size;
	uint32_t reserved;
} a65_object_payload_metadata_t;

typedef struct __attribute__((packed)) {
	a65_object_payload_metadata_t metadata;
	a65_object_section_t section[0];
} a65_object_payload_t;

#endif // A65_OBJECT_TYPE_H_
