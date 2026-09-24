/*
 * Project Name - Project Description
 * Copyright (C) 2026
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
\
#include "test_macros.h"
#include "libadt/freelist.h"
#include "libadt/util.h"

typedef struct libadt_freelist freelist_t;

#define MAX libadt_util_max

void test_libadt_freelist_init(void)
{
	freelist_t freelist = libadt_freelist_init(
		sizeof(int),
		0
	);
	assert(freelist.vector.size == MAX(sizeof(int), sizeof(void**)));
	assert(!freelist.vector.buffer);
	assert(!freelist.free);

	freelist = libadt_freelist_init(
		sizeof(int),
		4
	);
	assert(freelist.vector.size == MAX(sizeof(int), sizeof(void**)));
	assert(freelist.vector.buffer);
	assert(freelist.vector.capacity == 4);
	assert(!freelist.free);

	freelist = libadt_freelist_free(freelist);
	assert(!libadt_vector_valid(freelist.vector));
}

void test_libadt_freelist_insert(void)
{
	freelist_t freelist = libadt_freelist_init(sizeof(int), 0);

	assert(libadt_vector_valid(freelist.vector));

	int data = 4;
	assert(-1 < libadt_freelist_insert(&freelist, &data));
	assert(freelist.vector.length == 1);
	assert(freelist.vector.capacity == 1);

	assert(-1 < libadt_freelist_insert(&freelist, &data));
	assert(freelist.vector.length == 2);
	assert(freelist.vector.capacity == 2);

	// quick test of libadt_freelist_index, since I cba to write a whole separate test
	// just for this one assert
	assert(libadt_freelist_index(freelist, 0) == libadt_vector_index(freelist.vector, 0));

	libadt_freelist_free(freelist);
}

void test_libadt_freelist_remove(void)
{
	freelist_t freelist = libadt_freelist_init(sizeof(int), 0);

	assert(libadt_vector_valid(freelist.vector));

	int data = 4;
	assert(-1 < libadt_freelist_insert(&freelist, &data));
	assert(-1 < libadt_freelist_insert(&freelist, &data));
	assert(freelist.free == 0);

	libadt_freelist_remove(&freelist, 1);
	assert(freelist.free == libadt_vector_index(freelist.vector, 1));
	assert(*(void**)libadt_vector_index(freelist.vector, 1) == 0);
	assert(libadt_freelist_index(freelist, 1) == 0);
	assert(libadt_freelist_index(freelist, 0) == libadt_vector_index(freelist.vector, 0));

	libadt_freelist_remove(&freelist, 0);
	assert(freelist.free == libadt_vector_index(freelist.vector, 0));
	assert(*(void**)libadt_vector_index(freelist.vector, 0) == libadt_vector_index(freelist.vector, 1));
	assert(*(void**)libadt_vector_index(freelist.vector, 1) == 0);
	assert(libadt_freelist_index(freelist, 0) == 0);

	assert(-1 < libadt_freelist_insert(&freelist, &data));
	assert(freelist.free == libadt_vector_index(freelist.vector, 1));
	assert(*(void**)libadt_vector_index(freelist.vector, 1) == 0);
	assert(freelist.vector.capacity == 2);
	assert(libadt_freelist_index(freelist, 1) == 0);

	libadt_freelist_free(freelist);
}

int main()
{
	test_libadt_freelist_init();
	test_libadt_freelist_insert();
	test_libadt_freelist_remove();
}
