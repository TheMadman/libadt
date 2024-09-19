#include "libadt/lptr.h"

#include <stdint.h>
#include <limits.h>

// The (function_name) syntax prevents macro expansion
struct libadt_lptr (libadt_lptr_calloc)(size_t nmemb, size_t size)
{
	return (struct libadt_lptr) {
		calloc(nmemb, size),
		(ssize_t)nmemb,
		(ssize_t)size,
	};
}

struct libadt_lptr (libadt_lptr_reallocarray)(
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

struct libadt_lptr (libadt_lptr_free)(struct libadt_lptr lptr)
{
	free(lptr.buffer);
	return (struct libadt_lptr) { 0 };
}

void *(libadt_lptr_raw)(struct libadt_lptr lptr)
{
	return lptr.buffer;
}

bool (libadt_lptr_allocated)(struct libadt_lptr lptr)
{
	return !!libadt_lptr_raw(lptr);
}

bool (libadt_lptr_in_bounds)(struct libadt_lptr lptr)
{
	return lptr.length > 0;
}

bool (libadt_lptr_valid)(struct libadt_lptr lptr)
{
	return libadt_lptr_allocated(lptr)
		&& libadt_lptr_in_bounds(lptr);
}

struct libadt_lptr (libadt_lptr_truncate)(
	struct libadt_lptr lptr,
	size_t length
)
{
	return (struct libadt_lptr) {
		lptr.buffer,
		lptr.size,
		(ssize_t)length
	};
}

struct libadt_lptr (libadt_lptr_index)(
	struct libadt_lptr lptr,
	ssize_t index
)
{
	const ssize_t byte_index = index * lptr.size;
	return (struct libadt_lptr) {
		(char*)lptr.buffer + byte_index,
		lptr.size,
		lptr.length - index,
	};
}
