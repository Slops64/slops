[BITS 64]
DEFAULT REL
section .text
	global _start
	global stack
	%include "debug.inc"
	%include "stivale2.inc"

_start:
	xor rdx, rdx
	lea rsi, [string]
	call debug_print
	; Setting up the stack, because apparently limine 
	; doesn't want to do that for us
	lea rsp, [stack_top]
	lea rbp, [stack_bottom]
	mov rdx, 1
	; Print bootloader informations
	lea rsi, [bootloader_infos]
	call debug_print
	; Print bootloader
	lea rsi, [rdi]
	call debug_print
	xor rdx, rdx
	; Print bootloader version
	lea rsi, [rdi+64]
	call debug_print
	; Get command line tag
	; mov rsi, 0xe5e76a1b4597a781
	; call get_tag
	.inf_loop:
	hlt
  	jmp .inf_loop


; The stivale header our kernel needs to be booted via limine
section .stivale2hdr
	; This is set to zero, so the bootloader will use the elf entry point
	entry_point: dq 0
	; This tells the bootloader where our stack is
	; Remember the stack grows downwards :)
	stack: dq stack_top
	flags: dq 011b
	tags: dq framebuffer_ht 


section .bss
	align 16
	stack_bottom:
	this_is_the_stack: resb 16384 ; 16 KiB
	stack_top:

section .rodata
	string: db "[DEBUG] This is a debug message", 0
	bootloader_infos: db "[DEBUG] The kernel was booted using", 0
