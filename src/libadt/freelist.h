/*
 * libadt - A library containing abstract data types
 * Copyright (C) 2026  Marcus Harrison
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

#ifndef LIBADT_FREELIST
#define LIBADT_FREELIST

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file
 */

#include "vector.h"

struct _libadt_freelist_node {
	struct _libadt_freelist_node *next;
};

/**
 * \brief A struct representing a vector with a free-list.
 *
 * A free-list is a region of allocated memory that
 * keeps track of, and re-uses, sections that have been
 * freed.
 */
struct libadt_freelist {
	struct libadt_vector vector;
	struct _libadt_freelist_node *free;
};

/**
 * \brief Initializes a free-list.
 *
 * \param size The size of each item in the list.
 * \param initial_capacity An initial amount of memory to
 * 	allocate.
 *
 * \returns A newly initialized free-list.
 */
inline struct libadt_freelist libadt_freelist_init(
	size_t size,
	size_t initial_capacity
)
{
	return (struct libadt_freelist){
		.vector = libadt_vector_init(
			size,
			initial_capacity
		),
		.free = NULL,
	};
}

/**
 * \brief Releases the memory associated with the given
 * 	free-list.
 */
inline struct libadt_freelist libadt_freelist_free(
	struct libadt_freelist list
)
{
	return (struct libadt_freelist){
		.vector = libadt_vector_free(list.vector),
		.free = NULL,
	};
};

inline void *libadt_freelist_index(
	struct libadt_freelist *list,
	size_t index
)
{
	return libadt_vector_index(list->vector, index);
}

/**
 * \brief Inserts the given data into the first available
 * 	free location, growing the allocated memory if necessary.
 *
 * This function can reallocate the buffer. Pointers into the existing
 * buffer are invalid after calling this function.
 *
 * \param list The list to append to.
 * \param data A pointer to the data to append.
 *
 * \returns An integer index for the newly-inserted data, or
 * 	-1 if an error occurred.
 */
int libadt_freelist_insert(
	struct libadt_freelist *list,
	const void *data
);

/**
 * \brief Performs a logical remove of the item at the given index.
 *
 * Pointers to existing entries in the list are stable between
 * calls of this function.
 *
 * \param list The list to remove from.
 * \param index The index of the item to remove.
 *
 * \returns 0 if successful, -1 otherwise.
 */
int libadt_freelist_remove(
	struct libadt_freelist *list,
	size_t index
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_FREELIST
