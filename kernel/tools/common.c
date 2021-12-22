#include <tools/common.h>
#include <tools/console.h>

// Write a byte out to the specified port.
void outb(u16 port, u8 value)
{
	asm volatile ("outb %1, %0"::"dN" (port), "a"(value));
}

void outw(u16 port, u16 value)
{
	asm volatile ("outw %1, %0"::"dN" (port), "a"(value));
}

u8 inb(u16 port)
{
	u8 ret;
	asm volatile ("inb %1, %0":"=a" (ret)
		      :"dN"(port));
	return ret;
}

u16 inw(u16 port)
{
	u16 ret;
	asm volatile ("inw %1, %0":"=a" (ret)
		      :"dN"(port));
	return ret;
}

extern void panic(const char *message, const char *file, u32 line)
{
	cli();
	pr_emerg("PANIC: %s at %s:%d", message, file, line);
	hlt();
}

extern void panic_assert(const char *file, u32 line, const char *desc)
{
	cli();
	pr_err("ASSERTION: %s at %s:%d", desc, file, line);
	hlt();
}

void cli(void)
{
	asm volatile ("cli");
}

void sti(void)
{
	asm volatile ("sti");
}

void hlt(void)
{
	do {
		asm volatile ("hlt");
	} while (1);
	__builtin_unreachable();
}
