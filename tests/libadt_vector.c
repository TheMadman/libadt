/*
 * libadt - A library containing abstract data types
 * Copyright (C) 2024   Marcus Harrison
 *
 * This program is free_vector software: you can redistribute it and/or modify
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
#include "libadt/vector.h"

#define init_vector libadt_vector_init
#define free_vector libadt_vector_free
#define identity libadt_vector_identity
#define append_n libadt_vector_append_n
#define append libadt_vector_append
#define index libadt_vector_index
#define vacuum libadt_vector_vacuum
#define vector_end libadt_vector_end
#define truncate libadt_vector_truncate
#define pop libadt_vector_pop
typedef struct libadt_vector vector;

void test_identity(void)
{
	vector
		a = init_vector(sizeof(int), 4),
		b = a,
		c = init_vector(sizeof(int), 4);

	assert(a.buffer);
	assert(b.buffer);
	assert(c.buffer);

	// tests macro
	assert(identity(a, b));

	// tests function call
	assert((identity)(a, b));

	// identical except pointers
	assert(!identity(a, c));
	assert(!(identity)(a, c));

	// not gonna test this combinatorially
	// yell at me later

	free_vector(a);
	free_vector(c);
}

void test_append_n(void)
{
	vector a = init_vector(sizeof(int), 4);

	assert(a.buffer);

	int data[4] = { 1, 2, 3, 4 };

	vector result = append_n(a, data, 4);

	// No reallocation should have taken place
	assert(a.buffer == result.buffer);
	assert(a.capacity == result.capacity);

	// But they should fail the identity check
	assert(!identity(result, a));

	assert(result.length == 4);
	assert(*(int*)index(result, 0) == 1);
	assert(*(int*)index(result, 1) == 2);
	assert(*(int*)index(result, 2) == 3);
	assert(*(int*)index(result, 3) == 4);

	result = append_n(result, data, 4);

	// Reallocates, might still have the same pointer
	assert(result.length == 8);
	assert(*(int*)index(result, 4) == 1);
	assert(*(int*)index(result, 5) == 2);
	assert(*(int*)index(result, 6) == 3);
	assert(*(int*)index(result, 7) == 4);

	free_vector(a);
}

void test_append_macro(void)
{
	vector a = init_vector(sizeof(int), 0);

	int data = 4;

	vector result = append(a, &data);

	assert(result.length == 1);
	assert(*(int*)index(result, 0) == 4);

	// Should cause the capacity to expand
	// beyond the length
	result = append(result, &data);
	result = append(result, &data);
	result = append(result, &data);
	result = append(result, &data);

	// Might change the growth policy in the
	// future, what we care about is that the
	// capacity is greater than the length
	assert(result.capacity > result.length);

	free_vector(a);
}

void test_append(void)
{
	vector a = init_vector(sizeof(int), 0);

	int data = 4;

	vector result = (append)(a, &data);

	assert(result.length == 1);
	assert(*(int*)index(result, 0) == 4);

	// Should cause the capacity to expand
	// beyond the length
	result = (append)(result, &data);
	result = (append)(result, &data);
	result = (append)(result, &data);
	result = (append)(result, &data);

	// Might change the growth policy in the
	// future, what we care about is that the
	// capacity is greater than the length
	assert(result.capacity > result.length);

	free_vector(a);
}

void test_vacuum(void)
{
	vector a = init_vector(sizeof(int), 10);

	assert(a.buffer);
	assert(a.capacity == 10);
	
	int data = 4;
	a = append(a, &data);

	assert(a.length == 1);

	a = vacuum(a);

	assert(a.buffer);
	assert(a.capacity == 1);

	free_vector(a);
}

void test_truncate(void)
{
	vector a = init_vector(sizeof(int), 0);

	assert(!a.buffer);

	a = truncate(a, 10);

	assert(a.buffer);
	assert(a.capacity == 10);

	free_vector(a);
}

void test_end(void)
{
	vector a = init_vector(sizeof(int), 10);

	assert(a.buffer);

	int data = 4;
	a = append(append(a, &data), &data);

	assert(a.length == 2);

	int
		*begin = index(a, 0),
		*end = vector_end(a);

	assert(end - begin == 2);

	// function version
	end = (vector_end)(a);

	assert(end - begin == 2);

	free_vector(a);
}

void test_pop(void)
{
	int
		input = 4,
		output = 0;

	vector a = append(init_vector(sizeof(int), 1), &input);

	assert(a.length == 1);
	assert(*(int*)index(a, 0) == 4);

	a = pop(a, &output);

	assert(a.length == 0);
	assert(output == 4);
}

int main()
{
	test_identity();
	test_append_n();
	test_append_macro();
	test_append();
	test_vacuum();
	test_pop();
}
