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
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include "libadt/str.h"

char TEST_STR[] = "Hello, world!";
wchar_t TEST_WSTR[] = L"Hello, world!";

void test_str(void)
{
	struct libadt_lptr result = libadt_str(TEST_STR);
	assert(result.buffer == TEST_STR);
	assert(result.size == 1);
	assert(result.length == (ssize_t)strlen(TEST_STR));
}

void test_wstr(void)
{
	struct libadt_lptr result = libadt_wstr(TEST_WSTR);
	assert(result.buffer == TEST_WSTR);
	assert(result.size == sizeof(*TEST_WSTR));
	assert(result.length == (ssize_t)wcslen(TEST_WSTR));
}

int main()
{
	test_str();
	test_wstr();
}
