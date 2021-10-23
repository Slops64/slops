#ifndef MM_H
#define MM_H

#include <types.h>
#include <list.h>

#define MIN_SLICE_SIZE (sizeof(struct heap_slice) + 1)
#define SLICE_MAGIC (0xBADBABE)

struct heap_slice {
	u32 magic;
	struct heap_slice *next;
	u8 used;
	u64 len; // len of the buf
	char buf[];
};

void track_region(void *start, u64 len);
void *kmalloc(u64 len);
void kfree(void *p);

#endif // MM_H