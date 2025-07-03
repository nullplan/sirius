.global memmove
.type memmove, @function
memmove:
    movq %rdi, %rax
    subq %rsi, %rax
    cmpq %rdx, %rax
    jae 1f
    std
    leaq -1(%rdi, %rdx), %rdi
    leaq -1(%rsi, %rdx), %rsi
    movq %rdx, %rcx
    rep
    movsb
    leaq 1(%rdi), %rax
    cld
    retq
1:
.hidden __memcpy_fwd
    jmp __memcpy_fwd
