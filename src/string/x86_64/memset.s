.global memset
.type memset, @function
memset:
    movq %rdi, %r8
    movb %sil, %al
    movq %rdx, %rcx
    rep
    stosb
    movq %r8, %rax
    retq
.size memset, . - memset
