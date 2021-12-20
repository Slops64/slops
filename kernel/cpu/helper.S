
trampoline: 
    popq %rbp

    movw $0x10, %ax

    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    movw $0x28, %ax
    ltrw %ax

    ret
	
.global gdt_flush
gdt_flush: 
	lgdt (%rdi)
    pushq %rbp
    movq %rsp, %rbp

    pushq $0x10
    pushq %rbp
    pushf
    pushq $0x8
	push $trampoline
	iretq

.global idt_flush
idt_flush: 
	lidt (%rdi)
    ret

