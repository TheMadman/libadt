#include "libadt/str.h"

#include <string.h>
#include <wchar.h>
#include <limits.h>

struct libadt_lptr libadt_str(char *const str)
{
	return (struct libadt_lptr) {
		.buffer = str,
		.size = sizeof(*str),
		.length = strlen(str),
	};
}

struct libadt_lptr libadt_wstr(wchar_t *const str)
{
	return (struct libadt_lptr) {
		.buffer = str,
		.size = sizeof(*str),
		.length = (ssize_t)wcslen(str),
	};
}
