%ifndef DEBUG_INC
    %define DEBUG_INC

; This prints a string found in rsi to the 0xe9 port
; ( the qemu/bochs debug port )
debug_print:
	cld
	.loop
	lodsb
	cmp al, BYTE 0
	je .done
	out 0xe9, al
	jmp .loop
.done
	mov al, BYTE " "
	out 0xe9, al
	cmp rdx, 0
	je .linefeed
	ret
.linefeed
	mov al, BYTE 10
	out 0xe9, al
	ret
%endif
