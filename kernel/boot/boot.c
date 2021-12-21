#include <boot/stivale2.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <misc/types.h>
#include <mm/mm.h>
#include <test/test.h>
#include <tools/common.h>
#include <tools/console.h>
#include <tools/string.h>
#include <tools/unistd.h>

extern void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id);

void _start(struct stivale2_struct *stivale2_struct)
{
	struct stivale2_struct_tag_memmap *memmap =
	    stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
	struct stivale2_struct_tag_framebuffer *fb_tag =
	    stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

	init_console(fb_tag->framebuffer_addr, fb_tag->framebuffer_width,
			fb_tag->framebuffer_height, fb_tag->framebuffer_pitch);
	puts("Welcome to Slops\n");
	#ifdef DEBUG
	klog(KERN_DEBUG, "test", "Hello world");
	#endif
	print_mem(memmap);
	pmm_init(memmap);
	gdt_init();
	idt_init();
	do_tests();

	klog(KERN_EMERG, "kernel", "End of kernel...");
	// We're done, just hang...
	hlt();
}
