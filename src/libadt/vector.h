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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBADT_VECTOR_H
#define LIBADT_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "init.h"

#include <stdbool.h>
#include <stddef.h>

// TODO: implement these functions as inline

/**
 * \file
 */

/**
 * \brief Represents a vector, or dynamic array.
 *
 * This data structure dynamically allocates memory
 * for its elements, resizing as new elements are
 * appended.
 *
 * The data structure will attempt to reduce the
 * number of reallocation calls necessary, by allocating
 * a larger buffer all at once on simple appends.
 *
 * \sa LIBADT_VECTOR_WITH
 */
struct libadt_vector {
	/**
	 * \brief A pointer to the managed memory.
	 */
	void *buffer;

	/**
	 * \brief The size of each element.
	 */
	size_t size;

	/**
	 * \brief The number of elements currently
	 * 	being stored.
	 */
	size_t length;

	/**
	 * \brief The total number of elements that
	 * 	the current buffer can store, before
	 * 	requiring a reallocation.
	 */
	size_t capacity;
};

/**
 * \public \memberof libadt_vector
 * \brief Constructs a new libadt_vector with the given
 * 	element size and initial capacity.
 *
 * If an initial capacity was provided but failed to
 * allocate, a vector failing libadt_vector_valid() is
 * returned.
 *
 * \param size The size of an individual element.
 * \param initial_capacity The initial capacity to allocate.
 * 	An initial_capacity of 0 will delay allocation until
 * 	the vector is appended to.
 *
 * \returns A vector ready to append elements to, or a
 * 	vector failing libadt_vector_valid() if an allocation
 * 	attempt failed.
 */
EXPORT struct libadt_vector libadt_vector_init(size_t size, size_t initial_capacity);

/**
 * \public \memberof libadt_vector
 * \brief Frees the memory managed by the vector.
 *
 * \param vector The vector to free.
 *
 * \returns A vector failing libadt_vector_valid().
 */
EXPORT struct libadt_vector libadt_vector_free(struct libadt_vector vector);

/**
 * \public \memberof libadt_vector
 * \brief Tests whether a libadt_vector is a valid object.
 *
 * A function-like macro with the same name is provided, and will
 * be used by default for function call syntax.
 *
 * \param vector The vector to test.
 *
 * \returns True if the vector is valid for use, false otherwise.
 */
EXPORT bool libadt_vector_valid(struct libadt_vector vector);
#define libadt_vector_valid(vec) (!!(vec).size)

/**
 * \brief Provides a context manager interface for a vector.
 *
 * This macro creates a vector variable, with the name NAME,
 * initialized with the given SIZE and INITIAL_CAPACITY.
 *
 * If the vector initialized correctly, the given code block is run.
 * Otherwise, it is skipped.
 *
 * If the vector initialized correctly, then after the given
 * code block is run, the vector is released.
 *
 * Example usage:
 *
 * \code
 * LIBADT_VECTOR_WITH(my_vec, sizeof(int), 10) {
 * 	// Now, my_vec is a libadt_vector struct and can be used
 * 	// normally.
 *
 * 	my_data = 4;
 * 	my_vec = libadt_vector_append(my_vec, &my_data);
 *
 * 	// Breaking and returning from the block will produce
 * 	// a leak. To leave the block early while freeing the
 * 	// vector, use a continue instead.
 *
 * 	if (some_error)
 * 		continue;
 *
 * 	use(my_vec);
 * }
 * \endcode
 *
 * \param NAME The name to give the vector variable
 * \param SIZE The size of each element, as passed to libadt_vector_init()
 * \param INITIAL_CAPACITY The initial capacity of the vector, as passed
 * 	to libadt_vector_init()
 */
#define LIBADT_VECTOR_WITH(NAME, SIZE, INITIAL_CAPACITY) \
EXPORT for ( \
	struct libadt_vector \
		NAME = libadt_vector_init(SIZE, INITIAL_CAPACITY); \
	libadt_vector_valid(NAME); \
	NAME = libadt_vector_free(NAME) \
)

/**
 * \public \memberof libadt_vector
 * \brief Checks if the first vector and the second vector
 * 	refer to the same vector.
 *
 * Vectors are considered identical if the buffer, length, size and
 * capacity are the same.
 *
 * A function-like macro with the same name is provided, and will
 * be used by default for function call syntax.
 *
 * \param first The first vector to compare.
 * \param second The second vector to compare.
 *
 * \returns True if the vectors are identical, false otherwise.
 */
EXPORT bool libadt_vector_identity(
	struct libadt_vector first,
	struct libadt_vector second
);
#define libadt_vector_identity(first, second) \
	((first).buffer == (second).buffer \
	&& (first).size == (second).size \
	&& (first).length == (second).length \
	&& (first).capacity == (second).capacity)

/**
 * \public \memberof libadt_vector
 * \brief Appends _number_ new elements to the vector _vector_,
 * 	beginning from _data._
 *
 * If the append fails, the old vector will be returned. This
 * error can be checked with libadt_vector_identity(old, new).
 *
 * \param vector The vector to append elements to.
 * \param data The beginning of the elements to append.
 * \param number The number of elements to append.
 *
 * \returns A vector with the new data appended. If the append
 * 	failed, the old vector is returned.
 */
EXPORT struct libadt_vector libadt_vector_append_n(
	struct libadt_vector vector,
	void *data,
	size_t number
);

/**
 * \public \memberof libadt_vector
 * \brief Appends a single new element to the vector _vector_,
 * 	given by _data._
 *
 * Identical to libadt_vector_append_n(vector, data, 1).
 *
 * A function-like macro with the same name is provided, and will
 * be used by default for function call syntax.
 *
 * \param vector The vector to append the element to.
 * \param data The element to append.
 *
 * \returns A vector with the new element appended. On error,
 * 	the old vector is returned.
 */
EXPORT struct libadt_vector libadt_vector_append(
	struct libadt_vector vector,
	void *data
);
#define libadt_vector_append(vec, data) \
	libadt_vector_append_n((vec), (data), 1)

/**
 * \public \memberof libadt_vector
 * \brief Reallocates the vector's buffer down to the
 * 	size of the actually stored data.
 *
 * \param vector The vector to resize.
 */
EXPORT struct libadt_vector libadt_vector_vacuum(struct libadt_vector vector);

/**
 * \public \memberof libadt_vector
 * \brief Resizes the vector's capacity to new_capacity.
 *
 * If the new capacity is shorter than the current length,
 * elements on the end of the vector are dropped and the
 * length is set to the new capacity. If the value is larger
 * than the current length, the new memory is uninitialized
 * and the length is untouched.
 *
 * \param vector The vector to modify.
 * \param new_capacity The new capacity to resize the
 * 	vector to.
 *
 * \returns The new vector, with the correct pointer and
 * 	capacity. If the truncate failed, the old vector is
 * 	returned.
 */
EXPORT struct libadt_vector libadt_vector_trunc(
	struct libadt_vector vector,
	size_t new_capacity
);

/**
 * \public \memberof libadt_vector
 * \brief Returns a pointer to the item at _index_ in the
 * 	vector _vector._
 *
 * No check is performed. You must compare against libadt_vector::length
 * or against libadt_vector_end().
 *
 * A function-like macro with the same name is provided, and will
 * be used by default for function call syntax.
 *
 * \param vector The vector to index into.
 * \param index The item index to get, starting from zero.
 *
 * \returns A pointer to the item at the given index.
 */
EXPORT void *libadt_vector_index(struct libadt_vector vector, size_t index);

// wow this is ugly
#define libadt_vector_index(vec, index) \
	((void *)&((char *)(vec).buffer)[(vec).size * (index)])

/**
 * \public \memberof libadt_vector
 * \brief Returns a pointer one past the end of the last
 * 	item in _vector._
 *
 * A function-like macro with the same name is provided, and will
 * be used by default for function call syntax.
 *
 * \param vector The vector to get the end of.
 *
 * \returns A pointer one past the end of the last element.
 */
EXPORT void *libadt_vector_end(struct libadt_vector vector);
#define libadt_vector_end(vec) \
	libadt_vector_index((vec), (vec).length)

/**
 * \public \memberof libadt_vector
 * \brief Writes the last element of the vector to _out_ and
 * 	removes it from _vector_.
 *
 * This function only performs a logical remove: the memory is
 * not modified. To reclaim the memory, use libadt_vector_vacuum()
 * on completion.
 *
 * \param vector The vector to pop a value from.
 * \param out The location to write the value to.
 *
 * \returns The vector with the modified length.
 */
EXPORT struct libadt_vector libadt_vector_pop(struct libadt_vector vector, void *out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_VECTOR_H
