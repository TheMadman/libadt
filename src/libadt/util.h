/*
 * Project Name - Project Description
 * Copyright (C) 2024
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

#ifndef LIBADT_UTIL
#define LIBADT_UTIL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file
 * \brief Utility functions and macros for raw C types.
 *
 * For libadt types, check their respective modules.
 */

/**
 * \brief Calculates the length of a static C array.
 */
#define libadt_util_arrlength(array) (sizeof(array) / (sizeof(array[0])))

/**
 * \brief Returns the smaller value. Double-evaluates arguments.
 */
#define libadt_util_min(a, b) ((a) < (b) ? (a) : (b))

/**
 * \brief Returns the larger value. Double-evaluates arguments.
 */
#define libadt_util_max(a, b) ((a) > (b) ? (a) : (b))

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBADT_UTIL
