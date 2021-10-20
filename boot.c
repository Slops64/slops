#include <stivale2.h>
#include <types.h>
#include <mm.h>
#include <console.h>

static u8 stack[8192];
extern void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id);

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
int _start(struct stivale2_struct *stivale2_struct)
{
	// Let's get the terminal structure tag from the bootloader.
	struct stivale2_struct_tag_terminal *term_str_tag;
	term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

	// Check if the tag was actually found.
	if (term_str_tag == NULL)
	{
		// It wasn't found, just hang...
		for (;;)
		{
			asm("hlt");
		}
	}

	// Let's get the address of the terminal write function.
	init_console((term_write_t *)term_str_tag->term_write);

	printk("Hello World");

	// We're done, just hang...
	for (;;)
	{
		asm("hlt");
	}
}