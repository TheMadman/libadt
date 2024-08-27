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

#ifndef LIBADT_BITWISE_ARRAY_H
#define LIBADT_BITWISE_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/**
 * \file
 */

/**
 * \brief Type definition for libadt_bitwise array elements.
 */
typedef unsigned char libadt_bitwise_array_bit;

/**
 * \brief A structure representing a libadt_bitwise array.
 *
 * A libadt_bitwise array is an array of fixed-width elements,
 * whose widths may be specified in terms of arbitrary
 * bit sizes (such as one-bit, 2-bit, 3-bit etc., up until
 * the number of bits in a word).
 *
 * The functions for libadt_bitwise_array will set and get the
 * values, correctly packed.
 *
 * This currently only supports unsigned values.
 */
struct libadt_bitwise_array {
	/**
	 * \brief The number of elements currently in the
	 * 	array.
	 */
	ssize_t length;

	/**
	 * \brief The bit width of each element.
	 */
	int width;

	/**
	 * \brief The buffer containing the data.
	 */
	libadt_bitwise_array_bit *bits;
};

/**
 * \brief Constructs a libadt_bitwise_array from an existing memory
 * 	buffer.
 *
 * \param size The size of the existing buffer.
 * \param width The number of bits in each element.
 * \param buffer A pointer to the beginning of the buffer.
 *
 * \returns An initialized array.
 */
struct libadt_bitwise_array libadt_bitwise_array_make(
	size_t size,
	int width,
	libadt_bitwise_array_bit *buffer
);

/**
 * \brief Allocates a new libadt_bitwise_array, using the given
 * 	length and width.
 *
 * These are owning structures: they must be freed by
 * passing them to libadt_bitwise_array_free() when no longer needed.
 *
 * \param length The number of elements to store in the
 * 	array.
 * \param width The amount of bits for each element.
 *
 * \returns An initialized array on success, or an array
 * 	failing libadt_bitwise_array_valid() on failure.
 */
struct libadt_bitwise_array libadt_bitwise_array_alloc(ssize_t length, int width);

/**
 * \brief Tests if a given array is valid.
 *
 * \param array The array to test.
 *
 * \returns True if the array is valid, false otherwise.
 */
bool libadt_bitwise_array_valid(struct libadt_bitwise_array array);

/**
 * \brief Frees the given array.
 *
 * \param array The array to free.
 */
void libadt_bitwise_array_free(struct libadt_bitwise_array array);

/**
 * \brief Retreives the number at the given position in the
 * 	array.
 *
 * \param array The array to index into.
 * \param index The 0-based index of the element to retrieve.
 *
 * \returns The number stored at the given element.
 */
unsigned int libadt_bitwise_array_get(
	struct libadt_bitwise_array array,
	ssize_t index
);

/**
 * \brief Sets the value at the given index. Setting values
 * 	greater than the bit-width supports is undefined
 * 	behaviour.
 *
 * \param array The array to set the value in.
 * \param index The location in the array to set the value at.
 * \param value The value to set.
 */
void libadt_bitwise_array_set(
	struct libadt_bitwise_array array,
	ssize_t index,
	unsigned int value
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_BITWISE_ARRAY_H
