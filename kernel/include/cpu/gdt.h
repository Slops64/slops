#ifndef GDT_H
#define GDT_H

#include <tools/common.h>

#define GDT_ENTRY_COUNT 5

#define GDT_SEGMENT (0b00010000)
#define GDT_PRESENT (0b10000000)
#define GDT_USER (0b01100000)
#define GDT_EXECUTABLE (0b00001000)
#define GDT_READWRITE (0b00000010)

#define GDT_LONG_MODE_GRANULARITY 0b0010

#define GDT_FLAGS 0b1100

struct tss
{
    u32 reserved;
    u64 rsp[3];
    u64 reserved0;
    u64 ist[7];
    u32 reserved1;
    u32 reserved2;
    u16 reserved3;
    u16 iopb_offset;
} __packed;

struct gdt_desc
{
    u16 size;
    u64 offset;
} __packed;

struct gdt_ent
{
    u16 limit0_15;
    u16 base0_15;
    u8 base16_23;
    u8 flags;
    u8 limit16_19 : 4;
    u8 granularity : 4;
    u8 base24_31;
} __packed;

struct gdt_tss
{
    u16 length;
    u16 base_low16;
    u8 base_mid8;
    u8 flags1;
    u8 flags2;
    u8 base_high8;
    u32 base_upper32;
    u32 reserved;
} __packed;

struct gdt_arr
{
    struct gdt_ent entries[GDT_ENTRY_COUNT];
    struct gdt_tss tss;
} __packed;

void gdt_init(void);

void gdt_flush(u64);

void tss_flush(u64);

#endif
