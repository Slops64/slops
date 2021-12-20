#include <boot/stivale2.h>
#include <misc/types.h>
#include <tools/common.h>

static u8 stack[8192];

__section(".stivale2hdr")
__used static struct stivale2_header stivale_hdr = {
	// use elf entry point
	.entry_point = 0,
	// start of the stack
	.stack = (u64) ((char *)stack + sizeof(stack)),
	// Bit 1, if set, causes the bootloader to return to us pointers in the
	// higher half, which we likely want.
	// Bit 2, if set, tells the bootloader to enable protected memory ranges,
	// that is, to respect the ELF PHDR mandated permissions for the executable's
	// segments.
	.flags = (1 << 0) | (1 << 1),
	//.flags = (1 << 1) | (1 << 2),
	// no tags specified to force text mode
};

// search for a tag
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id)
{
	struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
	for (;;) {
		// If the tag pointer is NULL (end of linked list), we did not find
		// the tag. Return NULL to signal this.
		if (current_tag == NULL)
			return NULL;

		// Check whether the identifier matches. If it does, return a pointer
		// to the matching tag.
		if (current_tag->identifier == id)
			return current_tag;

		// Get a pointer to the next tag in the linked list and repeat.
		current_tag = (void *)current_tag->next;
	}
}
