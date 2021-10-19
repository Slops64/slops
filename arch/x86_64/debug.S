# This prints a string found in rsi to the 0xe9 port
# ( the qemu/bochs debug port )
.globl debug_print

debug_print: 
        cld
        debug_print.loop: 
        lodsb
        cmpb $0,%al
        je debug_print.done
        outb %al, $0xe9
        jmp debug_print.loop
debug_print.done: 
		mov $32, %al
        outb %al, $0xe9
        cmpq $0,%rdx
        je debug_print.linefeed
        ret
debug_print.linefeed: 
        movb $10,%al
        outb %al, $0xe9
        ret

