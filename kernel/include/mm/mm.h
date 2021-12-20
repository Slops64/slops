#ifndef MM_H
#define MM_H

#include <misc/types.h>
#include <misc/list.h>
#include <boot/stivale2.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define MIN_SLICE_SIZE (sizeof(struct heap_slice) + 1)
#define SLICE_MAGIC (0xBADBABE)
#define ALIGN_UP(__number) (((__number) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(__number) ((__number) & ~(PAGE_SIZE - 1))
#define ADDR_PFN(addr) ((u64)addr >> PAGE_SHIFT)
#define PFN_ADDR(pfn) ((void *)(pfn << PAGE_SHIFT))

// Physical memory manager
void pmm_init(struct stivale2_struct_tag_memmap *memory_map);
void *alloc_pages(u64 page_count);
void free_pages(void *page_addr, u64 page_count);
void *calloc_pages(u64 page_count);
void *realloc_pages(void *old_page, u64 newpages_count, u64 oldpages_count);


// Heap
struct heap_slice {
	u32 magic;
	struct heap_slice *next;
	u8 used;
	u64 len; // len of the buf
	char buf[];
};

void print_mem(struct stivale2_struct_tag_memmap *memmap);
void track_region(void *start, u64 len);
void *kmalloc(u64 len);
void kfree(void *p);

#endif // MM_H
