#include "libadt/vector.h"

#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct libadt_vector libadt_vector_trunc(
	struct libadt_vector vector,
	size_t new_capacity
)
{
	void *attempt = realloc(vector.buffer, vector.size * new_capacity);

	if (attempt) {
		vector.buffer = attempt;
		vector.capacity = new_capacity;
		vector.length = MIN(vector.length, new_capacity);
	}

	return vector;
}

struct libadt_vector libadt_vector_init(size_t size, size_t initial_capacity)
{
	struct libadt_vector result = {
		.buffer = NULL,
		.size = size,
		.capacity = 0,
		.length = 0,
	};

	if (initial_capacity)
		result = libadt_vector_trunc(result, initial_capacity);

	return result;
}

struct libadt_vector libadt_vector_free(struct libadt_vector vector)
{
	free(vector.buffer);
	return (struct libadt_vector){ 0 };
}

#undef libadt_vector_identity
bool libadt_vector_identity(
	struct libadt_vector first,
	struct libadt_vector second
)
{
	return first.buffer == second.buffer
		&& first.size == second.size
		&& first.length == second.length
		&& first.capacity == second.capacity;
}

struct libadt_vector libadt_vector_append_n(
	struct libadt_vector vector,
	void *data,
	size_t number
)
{
	if (number + vector.length > vector.capacity) {
		size_t new_capacity = MAX(
			vector.capacity * 2,
			vector.capacity + number
		);

		struct libadt_vector
			new = libadt_vector_trunc(vector, new_capacity);

		if (libadt_vector_identity(new, vector))
			return vector;
		vector = new;
	}

	memmove(libadt_vector_end(vector), data, vector.size * number);
	vector.length += number;
	return vector;
}

#undef libadt_vector_append
struct libadt_vector libadt_vector_append(
	struct libadt_vector vector,
	void *data
)
{
	return libadt_vector_append_n(vector, data, 1);
}

struct libadt_vector libadt_vector_vacuum(struct libadt_vector vector)
{
	return libadt_vector_trunc(vector, vector.length);
}

#undef libadt_vector_index
void *libadt_vector_index(struct libadt_vector vector, size_t index)
{
	return &((char *)vector.buffer)[vector.size * index];
}

#undef libadt_vector_end
void *libadt_vector_end(struct libadt_vector vector)
{
	return libadt_vector_index(vector, vector.length);
}