#ifndef IDT_H
#define IDT_H

#include <types.h>

#define ISR_N 256

struct idt_desc {
	u16	limit;
	u64	base;
} __attribute__((packed));

struct idt_gate {
	u16	base_low;
	u16	selector;
	u8	reserved_ist;
	u8	flags;
	u16	base_mid;
	u32	base_high;
	u32	reserved;
} __attribute__((packed));

struct registers {
	u64 ds;			// Data segment selector
	u64 rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;	// Pushed by pusha.
	u64 int_no, err_code;	// Interrupt number and error code (if applicable)
	u64 rip, cs, eflags, useresp, ss;	// Pushed by the processor automatically.
};

typedef void (*isr_t)(struct registers);

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

void init_idt(void);
void register_interrupt_handler(u8 n, isr_t handler);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#endif // IDT_H
