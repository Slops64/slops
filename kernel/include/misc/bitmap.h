#ifndef BITMAP_H
#define BITMAP_H

#include <misc/types.h>

static inline u64 get_bitmap_array_index(u64 page_addr)
{
    return page_addr / 8;
}

static inline u64 get_bitmap_bit_index(u64 page_addr)
{
    return page_addr % 8;
}

static inline bool bitmap_is_bit_set(u8 *bitmap, u64 page_addr)
{
    u64 bit = get_bitmap_bit_index(page_addr);
    u64 byte = get_bitmap_array_index(page_addr);
    return bitmap[byte] & (1 << bit);
}

static inline void bitmap_set_bit(u8 *bitmap, u64 page_addr)
{
    u64 bit = get_bitmap_bit_index(page_addr);
    u64 byte = get_bitmap_array_index(page_addr);
    bitmap[byte] |= (1 << bit);
}

static inline void bitmap_clear_bit(u8 *bitmap, u64 page_addr)
{
    u64 bit = get_bitmap_bit_index(page_addr);
    u64 byte = get_bitmap_array_index(page_addr);
    bitmap[byte] &= ~(1 << bit);
}

#endif