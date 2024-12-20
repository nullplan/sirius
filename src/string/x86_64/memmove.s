.global memmove
.type memmove, @function
memmove:
    movq %rdi, %rax
    subq %rsi, %rax
    cmpq %rdx, %rax
    jb 1f
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
    jmpq __memcpy_fwd