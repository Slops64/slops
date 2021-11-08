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
    for (u64 i = 0; i < memory_map->entries; i++)
    {
        if (memory_map->memmap[i].type != STIVALE2_MMAP_USABLE &&
            memory_map->memmap[i].type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE)
        {
            continue;
        }
        // Assert that checks if the start adress is page aligned
        ASSERT(memory_map->memmap[i].base % PAGE_SIZE == 0);
        u64 top = memory_map->memmap[i].base + memory_map->memmap[i].length;
        if (top < highest_page)
        {
            highest_page = top;
        }
    }
    size_t bitmap_size = ALIGN_UP(ALIGN_DOWN(highest_page) / PAGE_SIZE / 8);
    for (u64 i = 0; i < memory_map->entries; i++)
    {
        if (memory_map->memmap[i].type != STIVALE2_MMAP_USABLE)
        {
            continue;
        }
        // TODO: find the smallest possible region
        // that can contain the bitmap
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
            bitmap_clear_bit(ADDR_PFN(addr));
            free_memory += PAGE_SIZE;
        }
    }
    // Let's mark the bitmap area as used
    u64 bitmap_start = (u64)bitmap;
    u64 bitmap_end = bitmap_start + bitmap_size;
    for (u64 i = bitmap_start; i <= bitmap_end; i += PAGE_SIZE)
    {
        bitmap_set_bit(ADDR_PFN(i));
    }
    printk(" pmm -> Bitmap set up sucessfully, ready for allocations\n");
}

// TODO: refactor this
u64 find_free_pages(u64 count)
{
    ASSERT(count != 0);
    u64 free_count = 0;
    for (u64 i = last_free_page; i < ADDR_PFN(highest_page); i++)
    {
        while (bitmap[i / 8] == 0xff && i < ADDR_PFN(highest_page) - 8)
        {
            free_count = 0;
            i += 8 - (i % 8);
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
        last_free_page = 0;
        return find_free_pages(count);
    }
    return -1;
}

void *alloc_pages(u64 count)
{
    u64 pfn = find_free_pages(count);
    if (pfn == (u64)-1)
    {
        PANIC("Cannot allocate pages");
    }
    for (u64 i = pfn; i < count + pfn; i++)
    {
        bitmap_set_bit(i);
    }
    return PFN_ADDR(pfn);
}

void free_pages(void *page_addr, u64 page_count)
{
    u64 target = ADDR_PFN(page_addr);
    for (u64 i = target; i <= target + page_count; i++)
    {
        bitmap_clear_bit(i);
    }
}

void *calloc_pages(u64 page_count)
{
    void *allocated = alloc_pages(page_count);
    memset(allocated, 0, page_count * PAGE_SIZE);
    return allocated;
}

void *realloc_pages(void *old_page, u64 newpages_count, u64 oldpages_count)
{
    void *new_page = alloc_pages(newpages_count);
    memcpy(new_page, old_page, oldpages_count * PAGE_SIZE);
    free_pages(old_page, oldpages_count);
    return new_page;
}
