#include <idt.h>
#include <common.h>

struct idt_gate idt[ISR_N];
isr_t interrupt_handlers[ISR_N];

void set_idt_gate(u8 index, u64 base, u16 selector, u8 flags)
{
	idt[index].base_high = base >> 32;
	idt[index].base_mid = (base & 0xFFFFFFFF) >> 16;
	idt[index].base_low = base & 0xFFFF;

	idt[index].flags = flags;
	idt[index].selector = selector;
}

void lidt(struct idt_desc *idt)
{
	asm volatile("lidt %0" ::"m"(*idt));
}

void init_idt(void)
{
	struct idt_desc idt_d;

	// Remap the irq table.
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	set_idt_gate(0, (u64)isr0, 0x08, 0x8E);
	set_idt_gate(1, (u64)isr1, 0x08, 0x8E);
	set_idt_gate(2, (u64)isr2, 0x08, 0x8E);
	set_idt_gate(3, (u64)isr3, 0x08, 0x8E);
	set_idt_gate(4, (u64)isr4, 0x08, 0x8E);
	set_idt_gate(5, (u64)isr5, 0x08, 0x8E);
	set_idt_gate(6, (u64)isr6, 0x08, 0x8E);
	set_idt_gate(7, (u64)isr7, 0x08, 0x8E);
	set_idt_gate(8, (u64)isr8, 0x08, 0x8E);
	set_idt_gate(9, (u64)isr9, 0x08, 0x8E);
	set_idt_gate(10, (u64)isr10, 0x08, 0x8E);
	set_idt_gate(11, (u64)isr11, 0x08, 0x8E);
	set_idt_gate(12, (u64)isr12, 0x08, 0x8E);
	set_idt_gate(13, (u64)isr13, 0x08, 0x8E);
	set_idt_gate(14, (u64)isr14, 0x08, 0x8E);
	set_idt_gate(15, (u64)isr15, 0x08, 0x8E);
	set_idt_gate(16, (u64)isr16, 0x08, 0x8E);
	set_idt_gate(17, (u64)isr17, 0x08, 0x8E);
	set_idt_gate(18, (u64)isr18, 0x08, 0x8E);
	set_idt_gate(19, (u64)isr19, 0x08, 0x8E);
	set_idt_gate(20, (u64)isr20, 0x08, 0x8E);
	set_idt_gate(21, (u64)isr21, 0x08, 0x8E);
	set_idt_gate(22, (u64)isr22, 0x08, 0x8E);
	set_idt_gate(23, (u64)isr23, 0x08, 0x8E);
	set_idt_gate(24, (u64)isr24, 0x08, 0x8E);
	set_idt_gate(25, (u64)isr25, 0x08, 0x8E);
	set_idt_gate(26, (u64)isr26, 0x08, 0x8E);
	set_idt_gate(27, (u64)isr27, 0x08, 0x8E);
	set_idt_gate(28, (u64)isr28, 0x08, 0x8E);
	set_idt_gate(29, (u64)isr29, 0x08, 0x8E);
	set_idt_gate(30, (u64)isr30, 0x08, 0x8E);
	set_idt_gate(31, (u64)isr31, 0x08, 0x8E);
	set_idt_gate(32, (u64)irq0, 0x08, 0x8E);
	set_idt_gate(33, (u64)irq1, 0x08, 0x8E);
	set_idt_gate(34, (u64)irq2, 0x08, 0x8E);
	set_idt_gate(35, (u64)irq3, 0x08, 0x8E);
	set_idt_gate(36, (u64)irq4, 0x08, 0x8E);
	set_idt_gate(37, (u64)irq5, 0x08, 0x8E);
	set_idt_gate(38, (u64)irq6, 0x08, 0x8E);
	set_idt_gate(39, (u64)irq7, 0x08, 0x8E);
	set_idt_gate(40, (u64)irq8, 0x08, 0x8E);
	set_idt_gate(41, (u64)irq9, 0x08, 0x8E);
	set_idt_gate(42, (u64)irq10, 0x08, 0x8E);
	set_idt_gate(43, (u64)irq11, 0x08, 0x8E);
	set_idt_gate(44, (u64)irq12, 0x08, 0x8E);
	set_idt_gate(45, (u64)irq13, 0x08, 0x8E);
	set_idt_gate(46, (u64)irq14, 0x08, 0x8E);
	set_idt_gate(47, (u64)irq15, 0x08, 0x8E);

	idt_d.base = (u64)&idt;
	idt_d.limit = sizeof(idt) - 1;

	lidt(&idt_d);
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(struct registers regs)
{
	printk("interrupt occured\n");
	if (interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else
	{
		pr_err("unhandled interrupt: %d", regs.int_no);
	}
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(struct registers regs)
{
	printk("interrupt occured\n");
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no >= 40)
	{
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else
	{
		pr_warn("unhandled irq: %d", regs.int_no);
	}
}

void register_interrupt_handler(u8 n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}