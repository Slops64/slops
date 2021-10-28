#include <misc/types.h>
#include <mm/mm.h>
#include <tools/common.h>

static u8 *bitmap;
static u64 highest_page;
size_t bitmap_size;
u64 last_free_page = 0;

static inline u64 get_bitmap_array_index(u64 page_addr)
{
    return page_addr / 8;
}

static inline u64 get_bitmap_bit_index(u64 page_addr)
{
    return page_addr % 8;
}

static inline bool bitmap_is_bit_set(u64 page_addr)
{
    u64 bit = get_bitmap_bit_index(page_addr);
    u64 byte = get_bitmap_array_index(page_addr);
    return bitmap[byte] & (1 << bit);
}

static inline void bitmap_set_bit(u64 page_addr)
{
    u64 bit = get_bitmap_bit_index(page_addr);
    u64 byte = get_bitmap_array_index(page_addr);
    bitmap[byte] |= (1 << bit);
}

static inline void bitmap_clear_bit(u64 page_addr)
{
    u64 bit = get_bitmap_bit_index(page_addr);
    u64 byte = get_bitmap_array_index(page_addr);
    bitmap[byte] &= ~(1 << bit);
}

void pmm_init(struct stivale2_struct_tag_memmap *memory_map)
{
    // Assert that checks if the start adress is page aligned
    ASSERT(memory_map->memmap[1].base % PAGE_SIZE == 0);
    for (u64 i = 0; i < memory_map->entries; i++)
    {
        if (memory_map->memmap[i].type != STIVALE2_MMAP_USABLE &&
            memory_map->memmap[i].type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE)
        {
            continue;
        }
        u64 top = memory_map->memmap[i].base + memory_map->memmap[i].length;
        if (top < highest_page)
        {
            highest_page = top;
        }
    }
    size_t bitmap_size = ALIGN_DOWN(highest_page) / PAGE_SIZE / 8;
    for (u64 i = 0; i < memory_map->entries; i++)
    {
        if (memory_map->memmap[i].type != STIVALE2_MMAP_USABLE)
        {
            continue;
        }
        if (memory_map->memmap[i].length >= bitmap_size)
        {
            bitmap = (u8 *)(memory_map->memmap[i].base); // This may not be working
            goto bitmap_allocated;
        }
    }
    printk("Unable to allocate %d bytes for the bitmap\n", bitmap_size);
    PANIC("Could not allocate the bitmap");
bitmap_allocated:;
    u64 free_memory = 0;
    memset(bitmap, 0xff, bitmap_size);
    for (u64 i = 0; i < memory_map->entries; i++)
    {
        if (memory_map->memmap[i].type != STIVALE2_MMAP_USABLE)
        {
            continue;
        }
        for (u64 addr = memory_map->memmap[i].base; addr < memory_map->memmap[i].base + memory_map->memmap[i].length;
             addr += PAGE_SIZE)
        {
            bitmap_clear_bit(addr / PAGE_SIZE);
            free_memory += PAGE_SIZE;
        }
    }
    // Let's mark the bitmap area as used
    u64 bitmap_start = (u64)bitmap;
    u64 bitmap_end = bitmap_start + bitmap_size;
    for (u64 i = bitmap_start; i <= bitmap_end; i += PAGE_SIZE)
    {
        bitmap_set_bit(i / PAGE_SIZE);
    }
    printk(" pmm -> Bitmap set up sucessfully, ready for allocations\n");
}

u64 find_free_pages(u64 count)
{
    ASSERT(count!=0);
    u64 free_count = 0;
    for (u64 i = last_free_page; i < (highest_page / PAGE_SIZE); i++)
    {
	while (bitmap[i/8] == 0xff && i < (highest_page/PAGE_SIZE)-8)
	{
		free_count = 0;
		i += 8- (i % 8);
	}
        if (!bitmap_is_bit_set(i))
        {
            free_count++;
            if (free_count == count)
            {
		last_free_page = i;
                return i - count - 1;
            }
        }
        else
        {
            free_count = 0;
        }
    }
    if (last_free_page != 0)
    {
	    last_free_page =0;
	    return find_free_pages(count);
    }
    return -1;
}

void *alloc_pages(u64 count)
{
    u64 pfn = find_free_pages(count);
    if (pfn == 0)
    {
	    PANIC("Cannot allocate pages");
    }
    for (u64 i = pfn; i < count + pfn; i++)
    {
        bitmap_set_bit(i);
    }
    return (void *)(pfn * PAGE_SIZE);
}

void free_pages(void *page_addr, u64 page_count)
{
    u64 target = ((u64)page_addr) / PAGE_SIZE;
    for (u64 i = target; i <= target + page_count; i++)
    {
        bitmap_clear_bit(i);
    }
}
