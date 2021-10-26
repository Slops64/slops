#include <common.h>
#include <mm.h>

void do_tests(void)
{
	char *buf = kmalloc(12);
	strcpy(buf, "test1");
	printk("%s\n", buf);
	kfree(buf);

	asm volatile("int $0");
	asm volatile("int $0x10");
	void* page = alloc_page(1);
	strcpy(page, "test2");
	printk("%s\n", page);
	free_page(page, 1);
	printk("%s\n", page);
	// if test2 is displayed twice, there was a problem
}
