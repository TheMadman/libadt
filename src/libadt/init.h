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

#ifndef LIBADT_INIT
#define LIBADT_INIT

// Mark functions as external
// We don't have compiler-specific flags
// here: just assume the human compiling
// the project knows the compiler and has
// configured the environment.
//
// If the environment isn't set up, just use
// the compiler defaults.
//
// For example: for GCC, set
// CFLAGS+="-fvisibility=hidden -fvisibility-inlines-hidden -DEXPORT='"'__attribute__((visibility("default")))'"'"
#ifndef EXPORT
#define EXPORT
#endif

#endif
