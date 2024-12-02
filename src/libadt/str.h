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

#ifndef LIBADT_STR
#define LIBADT_STR

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <wchar.h>
#include <limits.h>

#include "lptr.h"

/**
 * \file
 * \brief This module provies convenience functions
 * 	and macros around libadt_lptr structs for
 * 	character arrays.
 */

/**
 * \brief Initializes a libadt_const_lptr to a C string
 * 	literal.
 *
 * The pointer length doesn't include the null terminator.
 *
 * \param str A C string literal. Can also be a wide literal L"".
 *
 * \return A libadt_const_lptr to the string literal.
 */
#define libadt_str_literal(str) \
	((struct libadt_const_lptr){ (str), sizeof(str[0]), libadt_util_arrlength(str) - 1 })

/**
 * \brief Initializes a libadt_lptr to a null-terminated
 * 	string.
 *
 * The lptr length will not include the null terminator.
 *
 * libadt_lptr%s use signed lengths. Therefore, the string
 * must not be longer then SSIZE_MAX. If it is, an invalid
 * lilbadt_lptr is returned.
 *
 * \param str A null-terminated string.
 *
 * \return A libadt_lptr to the null-terminated string.
 *
 * \sa libadt_wstr
 */
inline struct libadt_lptr libadt_str(char *const str)
{
	return (struct libadt_lptr) {
		.buffer = str,
		.size = sizeof(*str),
		.length = (ssize_t)strlen(str),
	};
}

/**
 * \brief Initializes a libadt_lptr to a null-terminated
 * 	wide character string.
 *
 * The lptr length will not include the null terminator.
 *
 * libadt_lptr%s use signed lengths. Therefore, the string
 * must not be longer then SSIZE_MAX. If it is, an invalid
 * lilbadt_lptr is returned.
 *
 * \param str A null-terminated wide character string.
 *
 * \return A libadt_lptr to the null-terminated string.
 *
 * \sa libadt_str
 */
inline struct libadt_lptr libadt_wstr(wchar_t *const str)
{
	return (struct libadt_lptr) {
		.buffer = str,
		.size = sizeof(*str),
		.length = (ssize_t)wcslen(str),
	};
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_STR
