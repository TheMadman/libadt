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
#include <limits.h>
#include <string.h>

#include "util.h"

/**
 * \brief Defines a constant long- or length-pointer type.
 *
 * Note that this implementation is _only_ intended to
 * extend the basic pointer type with array information:
 * it _does not_ implement additional safety checks
 * over pointers.
 *
 * Passing values greater than SSIZE_MAX, even to functions
 * accepting a size_t and not a ssize_t, is undefined
 * behaviour.
 *
 * \sa libadt_lptr
 */
struct libadt_const_lptr {
	/**
	 * \brief A pointer to the memory.
	 */
	const void *buffer;

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
 *
 * \sa libadt_const_lptr
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
 * \brief Initializes a libadt_const_lptr from a libadt_lptr.
 *
 * \param ptr The pointer to make a const pointer from.
 *
 * \returns The const pointer.
 */
inline struct libadt_const_lptr libadt_const_lptr(struct libadt_lptr ptr)
{
	return (struct libadt_const_lptr) {
		.buffer = ptr.buffer,
		.size = ptr.size,
		.length = ptr.length,
	};
}

/**
 * \brief Casts away the constness of a libadt_const_lptr.
 *
 * Casting away const is unsafe. This function should be avoided
 * unless you know what you're doing.
 *
 * \param cptr The pointer to cast const away from.
 *
 * \returns A non-const libadt_lptr.
 */
inline struct libadt_lptr libadt_lptr_unconst_cast(
	struct libadt_const_lptr cptr
)
{
	// Needs changing if the structure of
	// lptr and const_lptr diverge
	return *(struct libadt_lptr *)&cptr;
}

/**
 * \brief A convenience macro for initializing
 * 	an lptr from an existing fixed-length array.
 *
 * Example:
 *
 * \code
 * const char message[] = "Hello, world!";
 * struct libadt_const_lptr ptr = libad_lptr_init_array(message);
 * \endcode
 *
 * \param array The array to initialize an lptr from.
 * \returns A new libadt_const_lptr object.
 */
#define libadt_const_lptr_init_array(array) \
	((struct libadt_const_lptr){ (array), (sizeof(array[0])), libadt_util_arrlength(array) })

/**
 * \brief Returns raw pointer the given lptr contains.
 *
 * \param lptr The libadt_const_lptr to get the raw pointer for.
 *
 * \returns a void pointer.
 */
inline const void *libadt_const_lptr_raw(struct libadt_const_lptr lptr)
{
	return lptr.buffer;
}

/**
 * \brief Returns whether allocation of the lptr was
 * 	successful.
 *
 * Only valid for an lptr returned by libadt_lptr_calloc()
 * or libadt_lptr_realloc().
 *
 * \param lptr The lptr to test.
 *
 * \returns true if allocation succeeded, false otherwise.
 */
inline bool libadt_const_lptr_allocated(struct libadt_const_lptr lptr)
{
	return !!libadt_const_lptr_raw(lptr);
}

/**
 * \brief Returns whether the given lptr is out-of-bounds.
 *
 * Used in combination with libadt_const_lptr_index() to perform
 * boundary checking.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if the pointer is still in-bounds,
 * 	false if it is out-of-bounds.
 */
inline bool libadt_const_lptr_in_bounds(struct libadt_const_lptr lptr)
{
	return lptr.length > 0;
}

/**
 * \brief Returns whether the given lptr is valid.
 *
 * An lptr can be invalid if an allocation failed or if
 * it points to out-of-bounds memory.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if the pointer is valid, false otherwise.
 */
inline bool libadt_const_lptr_valid(struct libadt_const_lptr lptr)
{
	return libadt_const_lptr_allocated(lptr)
		&& libadt_const_lptr_in_bounds(lptr);
}

/**
 * \brief Changes the length of the given lptr to the given
 * 	value.
 *
 * Does not modify the allocation: to reallocate an allocated
 * lptr to a new size, use libadt_const_lptr_reallocarray().
 *
 * The primary use is to reduce the length of the buffer,
 * so that functions can operate on a sub-set of the buffer.
 *
 * Do not use this to increase the length of the buffer.
 *
 * \param lptr The lptr to modify.
 * \param length The new length to set.
 *
 * \returns A new libadt_const_lptr object, with the length reduced
 * 	if the new length was valid, or unmodified if not.
 */
inline struct libadt_const_lptr libadt_const_lptr_truncate(
	struct libadt_const_lptr lptr,
	size_t length
)
{
	return (struct libadt_const_lptr) {
		lptr.buffer,
		lptr.size,
		(ssize_t)length
	};
}

/**
 * \brief Progresses the lptr to the given index,
 * 	taking into account the member size.
 *
 * This function does no overflow check on `lptr.size * index`.
 *
 * This function does no boundary checking; test the
 * results with libadt_const_lptr_valid() or libadt_const_lptr_in_bounds().
 *
 * \param lptr The lptr to index into.
 * \param index The index.
 *
 * \returns A new libadt_const_lptr object, with the pointer
 * 	and length modified to correspond to the new
 * 	index.
 */
inline struct libadt_const_lptr libadt_const_lptr_index(
	struct libadt_const_lptr lptr,
	ssize_t index
)
{
	const ssize_t byte_index = index * lptr.size;
	return (struct libadt_const_lptr) {
		(char*)lptr.buffer + byte_index,
		lptr.size,
		lptr.length - index,
	};
}

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
	((struct libadt_lptr){ (array), (sizeof(array[0])), libadt_util_arrlength(array) })

/**
 * \brief Allocates an array buffer, initialized to 0,
 * 	returning it as an lptr.
 *
 * \param nmemb The number of members (length), identical to
 * 	calloc.
 * \param size The size of each member.
 *
 * \returns A new libadt_lptr object passing libadt_lptr_valid()
 * 	if allocation succeeded, or failing libadt_lptr_valid() if
 * 	allocation failed.
 */
inline struct libadt_lptr libadt_lptr_calloc(size_t nmemb, size_t size)
{
	return (struct libadt_lptr) {
		calloc(nmemb, size),
		(ssize_t)nmemb,
		(ssize_t)size,
	};
}

/**
 * \brief Reallocates an lptr, reusing the old size.
 *
 * \param lptr The lptr to reallocate.
 * \param nmemb The new number of members (length) to
 * 	reallocate the pointer to.
 *
 * \returns A new libadt_ptr object, either containing the
 * 	new length if reallocation was successful or the
 * 	old length if it failed.
 */
inline struct libadt_lptr libadt_lptr_reallocarray(
	struct libadt_lptr lptr,
	size_t nmemb
)
{
	if (SSIZE_MAX / (ssize_t)nmemb < lptr.size)
		return lptr;

	const size_t new_size = (size_t)lptr.size * nmemb;
	void *const attempt = realloc(lptr.buffer, new_size);
	if (attempt) {
		lptr.buffer = attempt;
		lptr.length = (ssize_t)nmemb;
	}
	return lptr;
}

/**
 * \brief Frees an allocated lptr, returning an invalid lptr.
 *
 * \param lptr The lptr to free
 *
 * \returns A libadt_lptr failing libadt_lptr_valid().
 */
inline struct libadt_lptr libadt_lptr_free(struct libadt_lptr lptr)
{
	free(lptr.buffer);
	return (struct libadt_lptr) { 0 };
}

/**
 * \brief Returns raw pointer the given lptr contains.
 *
 * \param lptr The libadt_lptr to get the raw pointer for.
 *
 * \returns a void pointer.
 */
inline void *libadt_lptr_raw(struct libadt_lptr lptr)
{
	return lptr.buffer;
}

/**
 * \brief Returns whether allocation of the lptr was
 * 	successful.
 *
 * Only valid for an lptr returned by libadt_lptr_calloc()
 * or libadt_lptr_realloc().
 *
 * \param lptr The lptr to test.
 *
 * \returns true if allocation succeeded, false otherwise.
 */
inline bool libadt_lptr_allocated(struct libadt_lptr lptr)
{
	return libadt_const_lptr_allocated(
		libadt_const_lptr(lptr)
	);
}

/**
 * \brief Returns whether the given lptr is out-of-bounds.
 *
 * Used in combination with libadt_lptr_index() to perform
 * boundary checking.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if the pointer is still in-bounds,
 * 	false if it is out-of-bounds.
 */
inline bool libadt_lptr_in_bounds(struct libadt_lptr lptr)
{
	return libadt_const_lptr_in_bounds(
		libadt_const_lptr(lptr)
	);
}

/**
 * \brief Returns whether the given lptr is valid.
 *
 * An lptr can be invalid if an allocation failed or if
 * it points to out-of-bounds memory.
 *
 * \param lptr The lptr to test.
 *
 * \returns true if the pointer is valid, false otherwise.
 */
inline bool libadt_lptr_valid(struct libadt_lptr lptr)
{
	return libadt_lptr_allocated(lptr)
		&& libadt_lptr_in_bounds(lptr);
}

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
 * \param lptr The lptr to modify.
 * \param length The new length to set.
 *
 * \returns A new libadt_lptr object, with the length reduced
 * 	if the new length was valid, or unmodified if not.
 */
inline struct libadt_lptr libadt_lptr_truncate(
	struct libadt_lptr lptr,
	size_t length
)
{
	return libadt_lptr_unconst_cast(
		libadt_const_lptr_truncate(
		libadt_const_lptr(lptr), length
	));
}

/**
 * \brief Progresses the lptr to the given index,
 * 	taking into account the member size.
 *
 * This function does no overflow check on `lptr.size * index`.
 *
 * This function does no boundary checking; test the
 * results with libadt_lptr_valid() or libadt_lptr_in_bounds().
 *
 * \param lptr The lptr to index into.
 * \param index The index.
 *
 * \returns A new libadt_lptr object, with the pointer
 * 	and length modified to correspond to the new
 * 	index.
 */
inline struct libadt_lptr libadt_lptr_index(
	struct libadt_lptr lptr,
	ssize_t index
)
{
	return libadt_lptr_unconst_cast(
		libadt_const_lptr_index(
		libadt_const_lptr(lptr), index
	));
}

#define LIBADT_LPTR_WITH(name, length, size) \
	for ( \
		struct libadt_lptr name = libadt_lptr_calloc((length), (size)); \
		libadt_lptr_allocated(name); \
		libadt_lptr_free(name), name = (struct libadt_lptr){ 0 } \
	)

/**
 * \brief Gets the total size of the memory pointed to by the lptr.
 *
 * \param lptr The pointer to get the total size for.
 *
 * \returns The total size of the memory pointed to.
 */
inline ssize_t libadt_const_lptr_size(struct libadt_const_lptr lptr)
{
	return lptr.size * lptr.length;
}

/**
 * \brief Copies memory from src to dest.
 *
 * If src is smaller than dest, all of src is copied.
 *
 * If dest is smaller than src, only enough bytes are copied
 * to fill dest.
 *
 * This function must not be used for memory that can overlap.
 * For locations that can overlap, use libadt_lptr_memmove().
 *
 * \param dest The destination to copy memory to.
 * \param src The data to copy.
 *
 * \returns dest.
 */
inline struct libadt_lptr libadt_lptr_memcpy(
	struct libadt_lptr dest,
	struct libadt_const_lptr src
) {
	const ssize_t
		dest_size = libadt_const_lptr_size(libadt_const_lptr(dest)),
		src_size = libadt_const_lptr_size(src),
		limit = dest_size < src_size ? dest_size : src_size;

	memcpy(dest.buffer, src.buffer, (size_t)limit);
	return dest;
}

/**
 * \brief Copies memory from src to dest.
 *
 * If src is smaller than dest, all of src is copied.
 *
 * If dest is smaller than src, only enough bytes are copied
 * to fill dest.
 *
 * Supports overlapping memory regions.
 *
 * \param dest The destination to copy memory to.
 * \param src The data to copy.
 *
 * \returns dest.
 */
inline struct libadt_lptr libadt_lptr_memmove(
	struct libadt_lptr dest,
	struct libadt_const_lptr src
) {
	const ssize_t
		dest_size = libadt_const_lptr_size(libadt_const_lptr(dest)),
		src_size = libadt_const_lptr_size(src),
		limit = dest_size < src_size ? dest_size : src_size;

	memmove(dest.buffer, src.buffer, (size_t)limit);
	return dest;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_LPTR
