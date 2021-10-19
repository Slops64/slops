# Tags needed to get awesome stuff from the bootloader:
# The kernel executable provides the bootloader with a linked list of structs,
# the first of which is pointed to by the tags entry of the stivale2 header.
# A structure tag is returned to the kernel by the bootloader
# While a header tag is used to ask the bootloader something

# Framebuffer header tag
.globl framebuffer_ht
framebuffer_ht: 
        framebuffer_ht.identifier: .quad 0x3ecc1bc43d0f7971
        framebuffer_ht.next: .quad 0
        framebuffer_ht.framebuffer_width: .word 0 # If fb infos are set to zero,
        framebuffer_ht.framebuffer_height: .word 0 # the bootloader will pick up
        framebuffer_ht.framebuffer_bpp: .word 0 # the bests for us.
        framebuffer_ht.unused: .word 0

# This will allow us to scan for tags that we want
# from the bootloader (stuctures tags).
# The stivale2 structure must be in rdi,
# and the requested tag in rsi.
# We return a pointer to the requested tag
# NOT FINISHED YET
get_tag: 
        # We put the first structure tag address in rdx
        leaq 128(%rdi),%rdx
        get_tag.loop: 
        cmpq $0,%rdx
        je get_tag.end
        cmpq %rsi,%rdx
        jne get_tag.loop
        movq %rdx,%rax
get_tag.done: 
        ret
get_tag.end: 
        movq $0,%rax
        je get_tag.done

