#include <boot/stivale2.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <graphics/framebuffer.h>
#include <misc/types.h>
#include <mm/mm.h>
#include <test/test.h>
#include <tools/common.h>
#include <tools/console.h>
#include <tools/string.h>

extern void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id);

void _start(struct stivale2_struct *stivale2_struct)
{
	// Let's get the terminal structure tag from the bootloader.
	struct stivale2_struct_tag_textmode *console =
	    stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TEXTMODE_ID);
	struct stivale2_struct_tag_memmap *memmap =
	    stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
	// struct stivale2_struct_tag_framebuffer *fb_tag =
	// stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

	if (memmap == NULL) {
		PANIC
		    ("stivale2_struct_tag_memmap expected but not been recieved");
	}
	// if (fb_tag == NULL)
	//{
	//  PANIC("Framebuffer structure tag not recieved");
	// }

	// Let's get the address of the terminal write function.
	init_console(console);
	// init_fb(fb_tag);
	// set_bg(12, 5, 59);

	for (u64 i = 0; i < memmap->entries; i++)
		if (memmap->memmap[i].type == STIVALE2_MMAP_USABLE) {
			printk("usable memory = %x length = %x\n",
			       memmap->memmap[i].base,
			       memmap->memmap[i].length);
			track_region((void *)memmap->memmap[i].base,
				     memmap->memmap[i].length);
		}

	gdt_init();
	idt_init();

	pmm_init(memmap);
	do_tests();

	// We're done, just hang...
	hlt();
}
