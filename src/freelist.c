#include "libadt/freelist.h"

#include <unistd.h>
#include <string.h>
#include <assert.h>

typedef struct _libadt_freelist_node node_t;

int libadt_freelist_insert(
	struct libadt_freelist *restrict list,
	const void *restrict data
)
{
	if (list->free) {
		ssize_t bytes = (char*)list->free - (char*)list->vector.buffer;
		assert(0 <= bytes);
		if (bytes < 0)
			return -1;

		ssize_t index = bytes / list->vector.size;

		node_t *new_free = list->free->next;
		memcpy(list->free, data, list->vector.size);
		list->free = new_free;
		return index;
	}

	if (libadt_vector_push(&list->vector, data) < 0)
		return -1;

	return list->vector.length - 1;
}

int libadt_freelist_remove(
	struct libadt_freelist *restrict list,
	size_t index
)
{
	if (list->vector.length < index)
		return -1;

	const struct _libadt_freelist_node new_node = {
		.next = list->free,
	};

	void *position = libadt_freelist_index(*list, index);
	*(struct _libadt_freelist_node*)position = new_node;
	list->free = position;
	return 0;
}

bool libadt_freelist_allocated(struct libadt_freelist list, size_t index)
{
	if (list.vector.length < index)
		return false;
	void *loc = libadt_vector_index(list.vector, index);
	for (struct _libadt_freelist_node *c = list.free; c; c = c->next) {
		if (c == loc)
			return false;
	}
	return true;
}

// non-inline definitions of inline functions in freelist.h
struct libadt_freelist libadt_freelist_init(
	size_t size,
	size_t initial_capacity
);
struct libadt_freelist libadt_freelist_free(
	struct libadt_freelist list
);
void *libadt_freelist_index(
	struct libadt_freelist list,
	size_t index
);
