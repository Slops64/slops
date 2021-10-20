#ifndef COMMON_H
#define COMMON_H

#include <types.h>
#include <console.h>

void outb(u16 port, u8 value);
u8 inb(u16 port);
u16 inw(u16 port);

#define PANIC(msg) (                    \
	{                                   \
		panic(msg, __FILE__, __LINE__); \
		__builtin_unreachable();        \
	})

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))
#define NULL ((void *)0)

void panic(const char *message, const char *file, u32 line);
void panic_assert(const char *file, u32 line, const char *desc);
void cli(void);
void sti(void);
void hlt(void);

#endif // COMMON_H
