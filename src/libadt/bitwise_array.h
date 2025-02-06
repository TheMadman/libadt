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

#ifndef LIBADT_BITWISE_ARRAY_H
#define LIBADT_BITWISE_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "init.h"

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>

#include "util.h"

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
EXTERN inline struct libadt_bitwise_array libadt_bitwise_array_alloc(ssize_t length, int width)
{
	if (length < 0 || width < 0)
		return (struct libadt_bitwise_array){ 0 };
	const lldiv_t division = lldiv(length * width, CHAR_BIT);
	const ssize_t bytes = division.quot + 1;

	return (struct libadt_bitwise_array) {
		.length = length,
		.width = width,
		.bits = malloc((size_t)bytes),
	};
}

/**
 * \brief Tests if a given array is valid.
 *
 * \param array The array to test.
 *
 * \returns True if the array is valid, false otherwise.
 */
EXTERN inline bool libadt_bitwise_array_valid(struct libadt_bitwise_array array)
{
	return array.bits != NULL;
}

/**
 * \brief Frees the given array.
 *
 * \param array The array to free.
 */
EXTERN inline void libadt_bitwise_array_free(struct libadt_bitwise_array array)
{
	free(array.bits);
}

/**
 * \brief Retreives the number at the given position in the
 * 	array.
 *
 * \param array The array to index into.
 * \param index The 0-based index of the element to retrieve.
 *
 * \returns The number stored at the given element.
 */
EXTERN inline unsigned int libadt_bitwise_array_get(
	struct libadt_bitwise_array array,
	ssize_t index
)
{
	/*
	 * This nonsense-function has to deal with the following
	 * three conditions:
	 * - The whole number is smaller than a byte and is in
	 *   the current byte
	 * - The whole number is smaller than a byte, but overlaps
	 *   into the next byte
	 * - The whole number is larger than a byte, potentially
	 *   up to a whole word size
	 *
	 * We have to start by finding how many bits into the byte
	 * the number starts from.
	 *
	 * We potentially have to shift the value to the right, if
	 * the number ends before the end of the byte.
	 *
	 * We have to mask out the bits we're not interested in.
	 *
	 * Then, in the case that there are still bits to read, we
	 * have to move on to the next byte.
	 *
	 * And we potentially have to do this for as many bytes
	 * as can fit in a single int.
	 *
	 * I did not expect this to be this difficult when I started.
	 *
	 * I also can't figure out how to write this so it's...
	 * less confusing. And I'm sorry.
	 */
	const lldiv_t byte_index = lldiv(index * array.width, CHAR_BIT);
	const libadt_bitwise_array_bit *location = &array.bits[byte_index.quot];

	unsigned result = 0;
	int start_from = (int)byte_index.rem;

	for (int bits_remaining = array.width; bits_remaining > 0; location++) {
		const libadt_bitwise_array_bit
			ones = (libadt_bitwise_array_bit)~0u,
			right_bits_ignore = (libadt_bitwise_array_bit)libadt_util_max(0, CHAR_BIT - start_from - bits_remaining),
			mask_right = ones >> start_from,
			mask_left = ones << right_bits_ignore,
			mask = mask_left & mask_right,
			bits_read = (libadt_bitwise_array_bit)(CHAR_BIT - start_from - right_bits_ignore),
			value_here = (*location & mask) >> right_bits_ignore;

		result = (result << bits_read) + value_here;

		bits_remaining -= bits_read;
		start_from = 0;
	}

	return result;
}

/**
 * \brief Sets the value at the given index. Setting values
 * 	greater than the bit-width supports is undefined
 * 	behaviour.
 *
 * \param array The array to set the value in.
 * \param index The location in the array to set the value at.
 * \param value The value to set.
 */
EXTERN inline void libadt_bitwise_array_set(
	struct libadt_bitwise_array array,
	ssize_t index,
	unsigned int value
)
{
	const lldiv_t byte_index = lldiv(index * array.width, CHAR_BIT);
	libadt_bitwise_array_bit *location = &array.bits[byte_index.quot];

	int start_from = (int)byte_index.rem;

	for (int bits_remaining = array.width; bits_remaining > 0; location++) {
		const libadt_bitwise_array_bit
			ones = (libadt_bitwise_array_bit)~0u,
			right_bits_ignore = (libadt_bitwise_array_bit)libadt_util_max(0, (CHAR_BIT - start_from - bits_remaining)),
			mask_right = ones >> start_from,
			mask_left = ones << right_bits_ignore,
			mask = ~(mask_left & mask_right),
			bits_write = (libadt_bitwise_array_bit)(CHAR_BIT - start_from - right_bits_ignore);

		*location = (libadt_bitwise_array_bit)((*location & mask)
			+ (value >> (bits_remaining - bits_write) << right_bits_ignore));

		bits_remaining -= bits_write;
		start_from = 0;
	}
}
#undef libadt_util_max

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_BITWISE_ARRAY_H
