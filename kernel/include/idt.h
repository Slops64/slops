#ifndef IDT_H
#define IDT_H

#include <types.h>
#include <common.h>

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define INTGATE 0x8e
#define TRAPGATE 0xeF

#define IDT_USER 0b01100000

#define IDT_ENTRY_COUNT 256

struct idt_desc {
	u16	limit;
	u64	base;
} __packed;

struct idt_ent {
	u16	base_low;
	u16	cs;
	u8	reserved_ist;
	u8	flags;
	u16	base_mid;
	u32	base_high;
	u32	reserved;
} __packed;

struct registers {
	u64 r15;
    u64 r14;
    u64 r13;
    u64 r12;
    u64 r11;
    u64 r10;
    u64 r9;
    u64 r8;
    u64 rbp;
    u64 rdi;
    u64 rsi;
    u64 rdx;
    u64 rcx;
    u64 rbx;
    u64 rax;

    u64 int_no;
    u64 err;

    // the interrupt stackframe
    u64 rip;
    u64 cs;
    u64 rflags;
    u64 rsp;
    u64 ss;
} __packed;

typedef void (*isr_t)(struct registers *);

void register_interrupt_handler(u8 n, isr_t handler);

void idt_flush(u64);

void idt_init(void);

#endif // IDT_H
