/*
 * Bitter - A library for handling arrays of arbitrary bit-length numbers
 * Copyright (C) 2024   Marcus Harrison
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test_macros.h"
#include <libadt/bitwise_array.h>

void test_alloc_success()
{
	struct libadt_bitwise_array array = libadt_bitwise_array_alloc(4, 3);
	assert(libadt_bitwise_array_valid(array));

	libadt_bitwise_array_free(array);
}

void test_get_byte()
{
	struct libadt_bitwise_array array = libadt_bitwise_array_alloc(4, 8);
	assert(libadt_bitwise_array_valid(array));

	array.bits[0] = 0xff;
	array.bits[1] = 0x00;
	array.bits[2] = 0x10;
	array.bits[3] = 0xcc;

	assert(libadt_bitwise_array_get(array, 0) == 0xff);
	assert(libadt_bitwise_array_get(array, 1) == 0x00);
	assert(libadt_bitwise_array_get(array, 2) == 0x10);
	assert(libadt_bitwise_array_get(array, 3) == 0xcc);

	libadt_bitwise_array_free(array);
}

void test_get_small_overlap()
{
	struct libadt_bitwise_array array = libadt_bitwise_array_alloc(4, 3);
	assert(libadt_bitwise_array_valid(array));

	libadt_bitwise_array_set(array, 0, 07);
	libadt_bitwise_array_set(array, 1, 00);
	libadt_bitwise_array_set(array, 2, 05);
	libadt_bitwise_array_set(array, 3, 03);

	assert(libadt_bitwise_array_get(array, 0) == 07);
	assert(libadt_bitwise_array_get(array, 1) == 00);
	assert(libadt_bitwise_array_get(array, 2) == 05);
	assert(libadt_bitwise_array_get(array, 3) == 03);

	libadt_bitwise_array_free(array);
}

void test_get_large_overlap()
{
	struct libadt_bitwise_array array = libadt_bitwise_array_alloc(4, 15);
	assert(libadt_bitwise_array_valid(array));

	libadt_bitwise_array_set(array, 0, ~(~0U << 15));
	libadt_bitwise_array_set(array, 1, 0);
	libadt_bitwise_array_set(array, 2, 10);
	libadt_bitwise_array_set(array, 3, 1000);

	assert(libadt_bitwise_array_get(array, 0) == ~(~0U << 15));
	assert(libadt_bitwise_array_get(array, 1) == 0);
	assert(libadt_bitwise_array_get(array, 2) == 10);
	assert(libadt_bitwise_array_get(array, 3) == 1000);
}

int main()
{
	test_alloc_success();
	test_get_byte();
	test_get_small_overlap();
	test_get_large_overlap();
}
