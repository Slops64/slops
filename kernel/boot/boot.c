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

	init_console(console);
	puts("Welcome to Slops\n");
	klog(KERN_INFO, "test", "Hello world");
	print_mem(memmap);
	pmm_init(memmap);
	gdt_init();
	idt_init();
	do_tests();

	klog(KERN_EMERG, "kernel", "End of kernel...");
	// We're done, just hang...
	hlt();
}
