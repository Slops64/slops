#include <stivale2.h>
#include <types.h>
#include <common.h>

// We will now write a helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id)
{
	struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
	for (;;)
	{
		// If the tag pointer is NULL (end of linked list), we did not find
		// the tag. Return NULL to signal this.
		if (current_tag == NULL)
		{
			return NULL;
		}

		// Check whether the identifier matches. If it does, return a pointer
		// to the matching tag.
		if (current_tag->identifier == id)
		{
			return current_tag;
		}

		// Get a pointer to the next tag in the linked list and repeat.
		current_tag = (void *)current_tag->next;
	}
}