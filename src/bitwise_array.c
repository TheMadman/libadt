#include "libadt/bitwise_array.h"

struct libadt_bitwise_array libadt_bitwise_array_alloc(ssize_t length, int width);
bool libadt_bitwise_array_valid(struct libadt_bitwise_array array);
void libadt_bitwise_array_free(struct libadt_bitwise_array array);
unsigned int libadt_bitwise_array_get(
	struct libadt_bitwise_array array,
	ssize_t index
);
void libadt_bitwise_array_set(
	struct libadt_bitwise_array array,
	ssize_t index,
	unsigned int value
);

