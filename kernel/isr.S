#define ISR_NOERRCODE(irq)   	.globl isr##irq; \
                                isr##irq: \
                                cli; \
                                pushq $0; \
                                pushq $##irq; \
                                jmp isr_common_stub; 

#define ISR_ERRCODE(irq)	.global isr##irq; \
				isr##irq: \
				cli; \
				pushq $##irq; \
				jmp isr_common_stub; 


#define IRQ(i, n)	.global irq##i; \
				irq##i: \
				cli; \
                pushq $0; \
                pushq $##n; \
				jmp irq_common_stub; 
        
#define PUSHQ 	pushq %rax;	\
				pushq %rcx;	\
				pushq %rdx;	\
				pushq %rbx;	\
				pushq %rsp;	\
				pushq %rbp;	\
				pushq %rsi;	\
				pushq %rdi;

#define POPQ 	popq %rdi;	\
				popq %rsi;	\
				popq %rbp;	\
				popq %rsp;	\
				popq %rbx;	\
				popq %rdx;	\
				popq %rcx;	\
				popq %rax;

ISR_NOERRCODE (0)
ISR_NOERRCODE (1)
ISR_NOERRCODE (2)
ISR_NOERRCODE (3)
ISR_NOERRCODE (4)
ISR_NOERRCODE (5)
ISR_NOERRCODE (6)
ISR_NOERRCODE (7)
ISR_ERRCODE   (8)
ISR_NOERRCODE (9)
ISR_ERRCODE   (10)
ISR_ERRCODE   (11)
ISR_ERRCODE   (12)
ISR_ERRCODE   (13)
ISR_ERRCODE   (14)
ISR_NOERRCODE (15)
ISR_NOERRCODE (16)
ISR_NOERRCODE (17)
ISR_NOERRCODE (18)
ISR_NOERRCODE (19)
ISR_NOERRCODE (20)
ISR_NOERRCODE (21)
ISR_NOERRCODE (22)
ISR_NOERRCODE (23)
ISR_NOERRCODE (24)
ISR_NOERRCODE (25)
ISR_NOERRCODE (26)
ISR_NOERRCODE (27)
ISR_NOERRCODE (28)
ISR_NOERRCODE (29)
ISR_NOERRCODE (30)
ISR_NOERRCODE (31)
/*IRQ Handler*/
IRQ (0, 32)
IRQ (1, 33)
IRQ (2, 34)
IRQ (3, 35)
IRQ (4, 36)
IRQ (5, 37)
IRQ (6, 38)
IRQ (7, 39)
IRQ (8, 40)
IRQ (9, 41)
IRQ (10, 42)
IRQ (11, 43)
IRQ (12, 44)
IRQ (13, 45)
IRQ (14, 46)
IRQ (15, 47)

# In isr.c
.extern isr_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    PUSHQ                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov %ds, %ax               # Lower 16-bits of eax = ds.
    push %rax                 # save the data segment descriptor

    mov $0x10, %ax  # load the kernel data segment descriptor
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call isr_handler

    pop %rbx        # reload the original data segment descriptor
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

    POPQ                     # Pops edi,esi,ebp...
    add $16, %rsp     # Cleans up the pushed err code and pushed ISR number
    # todo: try to remove this
    sti
    iretq           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
 

# In isr.c
.extern irq_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
irq_common_stub:
    PUSHQ                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov %ds, %ax               # Lower 16-bits of eax = ds.
    push %rax                 # save the data segment descriptor

    mov $0x10, %ax  # load the kernel data segment descriptor
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call irq_handler

    pop %rbx        # reload the original data segment descriptor
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

    POPQ                     # Pops edi,esi,ebp...
    add $16, %rsp     # Cleans up the pushed err code and pushed ISR number
    # todo: try to remove this
    sti
    iretq           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP