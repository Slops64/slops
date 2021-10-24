#include <gdt.h>

struct gdt_ent construct_gdt64_ent(u32 base, u32 limit, u8 granularity, u8 flags)
{
	struct gdt_ent gdt_ent = {};

	gdt_ent.limit0_15 = (u16)((limit)&0xffff);
	gdt_ent.base0_15 = (u16)((base)&0xffff);
	gdt_ent.base16_23 = (u8)(((base) >> 16) & 0xff);
	gdt_ent.flags = (flags);
	gdt_ent.limit16_19 = ((limit) >> 16) & 0x0f;
	gdt_ent.granularity = (granularity);
	gdt_ent.base24_31 = (u8)(((base) >> 24) & 0xff);

	return gdt_ent;
}

struct gdt_tss construct_gdt_tss(u64 tss_address)
{
	struct gdt_tss tss = {};

	tss.length = sizeof(struct tss);
	tss.base_low16 = tss_address & 0xffff;
	tss.base_mid8 = (tss_address >> 16) & 0xff;
	tss.flags1 = 0b10001001;
	tss.flags2 = 0;
	tss.base_high8 = (tss_address >> 24) & 0xff;
	tss.base_upper32 = tss_address >> 32;
	tss.reserved = 0;

	return tss;
}

static struct tss tss = {
	.reserved = 0,
	.rsp = {},
	.reserved0 = 0,
	.ist = {},
	.reserved1 = 0,
	.reserved2 = 0,
	.reserved3 = 0,
	.iopb_offset = 0,
};

static struct gdt_arr gdt;

static struct gdt_desc gdt_descriptor = {
	.size = sizeof(struct gdt_arr) - 1,
	.offset = (u64)&gdt,
};

void gdt_init()
{
	gdt.entries[0] = construct_gdt64_ent(0, 0, 0, 0); // null descriptor
	gdt.entries[1] = construct_gdt64_ent(0, 0, GDT_LONG_MODE_GRANULARITY, GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE);
	gdt.entries[2] = construct_gdt64_ent(0, 0, 0, GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE);

	gdt.entries[3] = construct_gdt64_ent(0, 0, GDT_LONG_MODE_GRANULARITY, GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER);
	gdt.entries[4] = construct_gdt64_ent(0, 0, 0, GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER);

	gdt.tss = construct_gdt_tss((u64)&tss);

	gdt_flush((u64)&gdt_descriptor);
}