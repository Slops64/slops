#include <tools/common.h>
#include <mm/mm.h>
#include <test/test.h>
#define MODULE_NAME "test"

void do_tests(void)
{
	char *buf = kmalloc(12);
	strcpy(buf, "test1");
	klog(KERN_INFO, MODULE_NAME, "kmalloc");
	kfree(buf);

	asm volatile ("int $0");
	asm volatile ("int $0x10");
	klog(KERN_INFO, MODULE_NAME, "interrupts");
	void *page = alloc_pages(1);
	strcpy(page, "test2");
	klog(KERN_INFO, MODULE_NAME, "allocating 1 page");
	free_pages(page, 1);
}
