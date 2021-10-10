%ifndef STIVALE2_INC
    %define STIVALE2_INC

; Tags needed to get awesome stuff from the bootloader:
; The kernel executable provides the bootloader with a linked list of structs,
; the first of which is pointed to by the tags entry of the stivale2 header.
; A structure tag is returned to the kernel by the bootloader
; While a header tag is used to ask the bootloader something

; Framebuffer header tag
framebuffer_ht:
	.identifier: dq 0x3ecc1bc43d0f7971
	.next: dq 0
	.framebuffer_width: dw 0 ; If fb infos are set to zero,
	.framebuffer_height: dw 0 ; the bootloader will pick up
	.framebuffer_bpp: dw 0 ; the bests for us.
	.unused: dw 0

; This will allow us to scan for tags that we want
; from the bootloader (stuctures tags).
; The stivale2 structure must be in rdi,
; and the requested tag in rsi.
; We return a pointer to the requested tag
; NOT FINISHED YET
get_tag:
	; We put the first structure tag address in rdx
	lea rdx, [rdi+128]
	.loop
	cmp rdx, 0
	je .end
	cmp rdx, rsi
	jne .loop
	mov rax, rdx
.done
	ret
.end
	mov rax, 0
	je .done

%endif
