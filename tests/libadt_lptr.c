/*
 * libadt - A library containing abstract data types
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <assert.h>
#include <stdint.h>
#include <limits.h>

#include "libadt/lptr.h"

typedef struct libadt_lptr lptr_t;

#define allocated libadt_lptr_allocated

void test_libadt_lptr_init_array(void)
{
	char message[] = "Hello, world!";
	lptr_t lptr = libadt_lptr_init_array(message);

	assert(lptr.length == sizeof(message));
	assert(lptr.size == 1);
	assert(lptr.buffer == message);
}

void test_libadt_lptr_calloc(void)
{
	{
		lptr_t lptr = libadt_lptr_calloc(4, sizeof(int));
		assert(allocated(lptr));

		libadt_lptr_free(lptr);
	}

	{
		// function version
		lptr_t lptr = (libadt_lptr_calloc)(4, sizeof(int));
		assert(allocated(lptr));

		libadt_lptr_free(lptr);
	}


	{
		// should fail
		lptr_t lptr = libadt_lptr_calloc(SSIZE_MAX, 1);
		assert(!allocated(lptr));
	}

	{
		//function version
		lptr_t lptr = (libadt_lptr_calloc)(SSIZE_MAX, 1);
		assert(!allocated(lptr));
	}
}

void test_libadt_lptr_reallocarray(void)
{
	lptr_t lptr = libadt_lptr_calloc(4, sizeof(int));
	assert(allocated(lptr));
	assert(lptr.length == 4);

	lptr = libadt_lptr_reallocarray(lptr, 8);
	assert(allocated(lptr));
	assert(lptr.length == 8);

	lptr = libadt_lptr_reallocarray(lptr, SSIZE_MAX);
	assert(allocated(lptr));
	assert(lptr.length == 8);
}

void test_libadt_lptr_free(void)
{
	{
		lptr_t lptr = libadt_lptr_calloc(4, sizeof(int));
		assert(allocated(lptr));

		lptr = libadt_lptr_free(lptr);
		assert(!allocated(lptr));
	}

	{
		lptr_t lptr = libadt_lptr_calloc(4, sizeof(int));
		assert(allocated(lptr));

		lptr = (libadt_lptr_free)(lptr);
		assert(!allocated(lptr));
	}
}

void test_libadt_lptr_raw(void)
{
	char message[] = "Hello, world!";
	lptr_t lptr = libadt_lptr_init_array(message);

	assert(libadt_lptr_raw(lptr) == message);
	assert((libadt_lptr_raw)(lptr) == message);
}

void test_libadt_lptr_truncate(void)
{
	lptr_t lptr = libadt_lptr_calloc(4, sizeof(int));

	assert(libadt_lptr_truncate(lptr, 2).length == 2);
	assert((libadt_lptr_truncate)(lptr, 2).length == 2);

	libadt_lptr_free(lptr);
}

int main()
{
	test_libadt_lptr_init_array();
	test_libadt_lptr_calloc();
	test_libadt_lptr_reallocarray();
	test_libadt_lptr_free();
	test_libadt_lptr_raw();
	test_libadt_lptr_truncate();
}
