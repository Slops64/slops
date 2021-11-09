#include <cpu/idt.h>
#include <tools/common.h>

extern u64 __interrupt_vector[];
struct idt_ent idt[IDT_ENTRY_COUNT];
struct idt_desc idt_descriptor = {
	.limit = sizeof(idt) - 1,
	.base = (u64) & idt[0],
};

isr_t interrupt_vector[IDT_ENTRY_COUNT];

struct idt_ent construct_idt_ent(u64 offset, u8 ist, u8 flags)
{
	struct idt_ent ent;

	ent.base_low = offset & 0xffff;
	ent.cs = 0x08;
	ent.reserved_ist = ist;
	ent.flags = flags;
	ent.base_mid = (offset >> 16) & 0xffff;
	ent.base_high = (offset >> 32) & 0xffffffff;
	ent.reserved = 0;
	return ent;
}

void idt_init()
{
	for (int i = 0; i < 48; i++)
		idt[i] = construct_idt_ent(__interrupt_vector[i], 0, INTGATE);

	idt[127] = construct_idt_ent(__interrupt_vector[48], 0, INTGATE);
	idt[128] =
	    construct_idt_ent(__interrupt_vector[49], 0, INTGATE | IDT_USER);

	idt_flush((u64) & idt_descriptor);
}

// This gets called from our ASM interrupt handler stub.
void interrupts_handler(struct registers *regs)
{
	if (interrupt_vector[regs->int_no] != 0) {
		isr_t handler = interrupt_vector[regs->int_no];
		handler(regs);
	}
}

void register_interrupt_handler(u8 n, isr_t handler)
{
	interrupt_vector[n] = handler;
}
