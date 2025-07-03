.global memmove
.type memmove, @function
memmove:
    movl %edi, %eax
    subl %esi, %eax
    cmpl %edx, %eax
    ja 1f
    std
    leaq -1(%rdi, %rdx), %rdi
    leaq -1(%rsi, %rdx), %rsi
    movl %edx, %ecx
    rep
    movsb
    leaq 1(%rdi), %rax
    cld
    retq
1:
.hidden __memcpy_fwd
    jmpq __memcpy_fwd
