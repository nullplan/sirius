.global memset
.type memset, @function
memset:
    movl %edi, %r8d
    movb %sil, %al
    movl %edx, %ecx
    rep
    stosb
    movl %r8d, %eax
    retq
.size memset, . - memset
