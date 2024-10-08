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
\
#ifndef LIBADT_LPTR
#define LIBADT_LPTR

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file
 * \copydoc libadt_lptr
 */

#include <stdlib.h>
#include <stdbool.h>

/**
 * \brief Defines a long- or length-pointer type.
 *
 * Note that this implementation is _only_ intended to
 * extend the basic pointer type with array information:
 * it _does not_ implement additional safety checks
 * over pointers.
 *
 * Passing values greater than SSIZE_MAX, even to functions
 * accepting a size_t and not a ssize_t, is undefined
 * behaviour.
 */
struct libadt_lptr {
	/**
	 * \brief A pointer to the memory.
	 */
	void *buffer;

	/**
	 * \brief The size of each member, used for
	 * 	indexing and iterating.
	 */
	ssize_t size;

	/**
	 * \brief The number of member objects the
	 * 	buffer can store.
	 */
	ssize_t length;
};

/**
 * \brief A convenience macro for initializing
 * 	an lptr from an existing fixed-length array.
 *
 * Example:
 *
 * \code
 * char message[] = "Hello, world!";
 * struct libadt_lptr ptr = libad_lptr_init_array(message);
 * \endcode
 *
 * \param array The array to initialize an lptr from.
 * \returns A new libadt_lptr object.
 */
#define libadt_lptr_init_array(array) \
	((struct libadt_lptr){ (array), (sizeof(array[0])), (sizeof(array) / (sizeof(array[0]))) })

/**
 * \brief Allocates an array buffer, initialized to 0,
 * 	returning it as an lptr.
 *
 * As well as a function, a macro is provided, wrapping
 * calloc() directly.
 *
 * \param nmemb The number of members (length), identical to
 * 	calloc.
 * \param size The size of each member.
 *
 * \returns A new libadt_lptr object passing libadt_lptr_valid()
 * 	if allocation succeeded, or failing libadt_lptr_valid() if
 * 	allocation failed.
 */
struct libadt_lptr libadt_lptr_calloc(size_t nmemb, size_t size);
#define libadt_lptr_calloc(nmemb, size) \
	((struct libadt_lptr) { \
		calloc((nmemb), (size)), \
		(ssize_t)(nmemb), \
		(ssize_t)(size)\
	})

/**
 * \brief Reallocates an lptr, reusing the old size.
 *
 * This function is not provided as a macro, as `reallocarray`
 * is a non-standard extension. This function performs an
 * overflow check on the new length.
 *
 * \param lptr The lptr to reallocate.
 * \param nmemb The new number of members (length) to
 * 	reallocate the pointer to.
 *
 * \returns A new libadt_ptr object, either containing the
 * 	new length if reallocation was successful or the
 * 	old length if it failed.
 */
struct libadt_lptr libadt_lptr_reallocarray(
	struct libadt_lptr lptr,
	size_t nmemb
);

/**
 * \brief Frees an allocated lptr, returning an invalid lptr.
 *
 * Also provided in macro form.
 *
 * \param lptr The lptr to free
 *
 * \returns A libadt_lptr failing libadt_lptr_valid().
 */
struct libadt_lptr libadt_lptr_free(struct libadt_lptr lptr);
#define libadt_lptr_free(lptr) \
	(free((lptr).buffer), (struct libadt_lptr) { 0 })

/**
 * \brief Returns raw pointer the given lptr contains.
 *
 * Also provided in macro form.
 *
 * \param lptr The libadt_lptr to get the raw pointer for.
 *
 * \returns a void pointer.
 */
void *libadt_lptr_raw(struct libadt_lptr lptr);
#define libadt_lptr_raw(lptr) ((lptr).buffer)

/**
 * \brief Returns whether allocation of the lptr was
 * 	successful.
 *
 * Only valid for an lptr returned by libadt_lptr_calloc()
 * or libadt_lptr_realloc().
 *
 * Also provided in macro form.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if allocation succeeded, false otherwise.
 */
bool libadt_lptr_allocated(struct libadt_lptr lptr);
#define libadt_lptr_allocated(lptr) (!!libadt_lptr_raw(lptr))

/**
 * \brief Returns whether the given lptr is out-of-bounds.
 *
 * Used in combination with libadt_lptr_index() to perform
 * boundary checking.
 *
 * Also provided in macro form.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if the pointer is still in-bounds,
 * 	false if it is out-of-bounds.
 */
bool libadt_lptr_in_bounds(struct libadt_lptr lptr);
#define libadt_lptr_in_bounds(lptr) ((lptr).length > 0)

/**
 * \brief Returns whether the given lptr is valid.
 *
 * An lptr can be invalid if an allocation failed or if
 * it points to out-of-bounds memory.
 *
 * Also provided in macro form.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if the pointer is valid, false otherwise.
 */
bool libadt_lptr_valid(struct libadt_lptr lptr);
#define libadt_lptr_valid(lptr) \
	(libadt_lptr_allocated(lptr) && libadt_lptr_in_bounds(lptr))

/**
 * \brief Changes the length of the given lptr to the given
 * 	value.
 *
 * Does not modify the allocation: to reallocate an allocated
 * lptr to a new size, use libadt_lptr_reallocarray().
 *
 * The primary use is to reduce the length of the buffer,
 * so that functions can operate on a sub-set of the buffer.
 *
 * Do not use this to increase the length of the buffer.
 *
 * Also provided in macro form.
 *
 * \param lptr The lptr to modify.
 * \param length The new length to set.
 *
 * \returns A new libadt_lptr object, with the length reduced
 * 	if the new length was valid, or unmodified if not.
 */
struct libadt_lptr libadt_lptr_truncate(
	struct libadt_lptr lptr,
	size_t length
);
#define libadt_lptr_truncate(lptr, length) \
	((struct libadt_lptr) { \
	 	(lptr).buffer, \
		(lptr).size, \
		(length), \
	})

/**
 * \brief Progresses the lptr to the given index,
 * 	taking into account the member size.
 *
 * This function does no overflow check on `lptr.size * index`.
 *
 * This function does no boundary checking; test the
 * results with libadt_lptr_valid() or libadt_lptr_in_bounds().
 *
 * Also provided in macro form.
 *
 * \param lptr The lptr to index into.
 * \param index The index.
 *
 * \returns A new libadt_lptr object, with the pointer
 * 	and length modified to correspond to the new
 * 	index.
 */
struct libadt_lptr libadt_lptr_index(
	struct libadt_lptr lptr,
	ssize_t index
);
#define libadt_lptr_index(lptr, index) \
	((struct libadt_lptr) { \
		(lptr).buffer + index * (lptr).size, \
		(lptr).size, \
		(lptr).length - index, \
	})

#define LIBADT_LPTR_WITH(name, length, size) \
	for ( \
		struct libadt_lptr name = libadt_lptr_calloc(length, size); \
		libadt_lptr_allocated(name); \
		libadt_lptr_free(name), name = (struct libadt_lptr){ 0 } \
	)
#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_LPTR
