.text
        .global _start
        .global stack

_start: 
        xorq %rdx, %rdx
        leaq string, %rsi
        call debug_print
        # Setting up the stack,  because apparently limine 
        # doesn't want to do that for us
        leaq stack_top, %rsp
        leaq stack_bottom, %rbp
        movq $1, %rdx
        # Print bootloader informations
        leaq bootloader_infos, %rsi
        call debug_print
        # Print bootloader
        leaq (%rdi), %rsi
        call debug_print
        xorq %rdx, %rdx
        # Print bootloader version
        leaq 64(%rdi), %rsi
        call debug_print
        # Get command line tag
        # mov rsi, 0xe5e76a1b4597a781
        # call get_tag
        _start.inf_loop: 
        hlt
        jmp _start.inf_loop


# The stivale header our kernel needs to be booted via limine
.section .stivale2hdr
        # This is set to zero, so the bootloader will use the elf entry point
        entry_point: 
			.quad 0
        # This tells the bootloader where our stack is
        # Remember the stack grows downwards :)
		stack: 
			.quad stack_top

		flags:
			.quad 3

		tags:
			.quad framebuffer_ht

.bss
	.align 16
	stack_bottom:
	this_is_the_stack: 
		.zero 16384 # 16 KiB
	stack_top:

.section .rodata
	string:
		.asciz "[DEBUG] This is a debug message"
	bootloader_infos:
		.asciz "[DEBUG] The kernel was booted using"
		