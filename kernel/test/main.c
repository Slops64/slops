#include <common.h>
#include <mm.h>

void do_tests(void)
{
	char *buf = kmalloc(12);
	strcpy(buf, "test");
	printk("%s\n", buf);
	kfree(buf);

	asm volatile("int $0");
	asm volatile("int $0x10");
}