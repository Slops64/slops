#include <stivale2.h>
#include <types.h>
#include <console.h>
#include <common.h>
#include <string.h>
#include <mm.h>
#include <idt.h>
#include <framebuffer.h>

static u8 stack[8192];
extern void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id);


void hello_isr(struct registers reg)
{
	printk("hello from irq %d\n", reg.int_no);
}

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
		.next = 0},
	// The terminal header tag possesses a flags field, leave it as 0 for now
	// as it is unused.
	.flags = 0};

// We are now going to define a framebuffer header tag.
// This tag tells the bootloader that we want a graphical framebuffer instead
// of a CGA-compatible text mode. Omitting this tag will make the bootloader
// default to text mode, if available.
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = (u64)&terminal_hdr_tag},
	// We set all the framebuffer specifics to 0 as we want the bootloader
	// to pick the best it can.
	.framebuffer_width = 0,
	.framebuffer_height = 0,
	.framebuffer_bpp = 0,
};

// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
	// The entry_point member is used to specify an alternative entry
	// point that the bootloader should jump to instead of the executable's
	// ELF entry point. We do not care about that so we leave it zeroed.
	.entry_point = 0,
	// Let's tell the bootloader where our stack is.
	// We need to add the sizeof(stack) since in x86(_64) the stack grows
	// downwards.
	.stack = (u64)((char *)stack + sizeof(stack)),
	// Bit 1, if set, causes the bootloader to return to us pointers in the
	// higher half, which we likely want.
	// Bit 2, if set, tells the bootloader to enable protected memory ranges,
	// that is, to respect the ELF PHDR mandated permissions for the executable's
	// segments.
	.flags = (1 << 0) | (1 << 1),
	//.flags = (1 << 1) | (1 << 2),
	// This header structure is the root of the linked list of header tags and
	// points to the first one in the linked list.
	.tags = (u64)((void *)&framebuffer_hdr_tag)};

// The following will be our kernel's entry point.
void _start(struct stivale2_struct *stivale2_struct)
{
	// Let's get the terminal structure tag from the bootloader.
	struct stivale2_struct_tag_terminal *term = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);
	struct stivale2_struct_tag_memmap *memmap = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
	struct stivale2_struct_tag_framebuffer *fb_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

	// Check if the tag was actually found.
	if (term == NULL)
	{
		PANIC("stivale2_struct_tag_terminal expected but not been recieved");
		// It wasn't found, just hang...
	}

	if (memmap == NULL)
	{
		PANIC("stivale2_struct_tag_memmap expected but not been recieved");
	}
	if (fb_tag == NULL)
	{
		PANIC("Framebuffer structure tag not recieved");
	}

	// Let's get the address of the terminal write function.
	init_console((term_write_t *)term->term_write);
	init_fb(fb_tag);
	set_bg(12, 5, 59);

	for (u64 i = 0; i < memmap->entries; i++)
	{
		if (memmap->memmap[i].type == STIVALE2_MMAP_USABLE)
		{
			printk("usable memory = %x length = %d\n", memmap->memmap[i].base, memmap->memmap[i].length);
			track_region((void *)memmap->memmap[i].base, memmap->memmap[i].length);
		}
	}
	printk("hello");
	char *buf = kmalloc(12);
	strcpy(buf, "test");
	printk("%s\n", buf);
	kfree(buf);

	init_idt();
	register_interrupt_handler(33, hello_isr);
	asm volatile("int $33");

	// We're done, just hang...
	hlt();
}
