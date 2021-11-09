#include <mm/mm.h>
#include <tools/common.h>

struct heap_slice *heap_start;

// NOTE: len refers to the size of the whole slice including the host struct
struct heap_slice *init_slice(struct heap_slice *s, u64 len,
			      struct heap_slice *next)
{
	s->magic = SLICE_MAGIC;
	s->next = next;
	s->len = len - sizeof(struct heap_slice);
	s->used = 0;
	return s;
}

void track_region(void *start, u64 len)
{
	struct heap_slice *last = heap_start;

	// called for the first time
	if (!heap_start)
		heap_start = init_slice(start, len, NULL);
	else {
		while (last->next)
			last = last->next;

		last->next = init_slice(start, len, NULL);
	}
}

struct heap_slice *get_slice(struct heap_slice *s)
{
	s->used = 1;
	return s;
}

struct heap_slice *put_slice(struct heap_slice *s)
{
	s->used = 0;
	return s;
}

void check_slice(struct heap_slice *s)
{
	if (s->magic != SLICE_MAGIC)
		PANIC("memory corruption");
}

void *kmalloc(u64 len)
{
	struct heap_slice *slice = heap_start;
	struct heap_slice *new_slice;

	while (slice) {
		check_slice(slice);
		if (slice->len < len)
			goto next;

		if (slice->len == len)
			return get_slice(slice)->buf;

		if (slice->len > len) {
			// split the slice
			if (slice->len - len >= MIN_SLICE_SIZE) {
				new_slice =
				    init_slice((struct heap_slice *)(slice->
								     buf + len),
					       slice->len - len, slice->next);

				slice->len = len;
				slice->next = new_slice;

				return get_slice(slice)->buf;
			} else
				return slice->buf;
		}

next:
		slice = slice->next;
	}
	track_region(alloc_pages(ALIGN_UP(len) / PAGE_SIZE), ALIGN_UP(len));
	
	// retry again
	return kmalloc(len);
}

void kfree(void *p)
{
	struct heap_slice *s = p - sizeof(struct heap_slice);

	check_slice(s);

	put_slice(s);

	// check the possiblity of merge with next
	if (s->next && !s->next->used) {
		s->next = s->next->next;
		s->len += s->next->len + sizeof(struct heap_slice);
	}
}
