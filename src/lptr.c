#include "libadt/lptr.h"

#include <stdint.h>

// This file just exposes the implementations in the
// .h file as external symbols in the shared object.
// There are deliberately no implementations here, just
// extern declarations.

struct libadt_const_lptr libadt_const_lptr(struct libadt_lptr ptr);
struct libadt_lptr libadt_lptr_calloc(size_t nmemb, size_t size);
struct libadt_lptr libadt_lptr_reallocarray(
	struct libadt_lptr lptr,
	size_t nmemb
);
struct libadt_lptr libadt_lptr_free(struct libadt_lptr lptr);
void *libadt_lptr_raw(struct libadt_lptr lptr);
bool libadt_lptr_allocated(struct libadt_lptr lptr);
bool libadt_lptr_in_bounds(struct libadt_lptr lptr);
bool libadt_lptr_valid(struct libadt_lptr lptr);
struct libadt_lptr libadt_lptr_truncate(
	struct libadt_lptr lptr,
	size_t length
);
struct libadt_lptr libadt_lptr_index(
	struct libadt_lptr lptr,
	ssize_t index
);
