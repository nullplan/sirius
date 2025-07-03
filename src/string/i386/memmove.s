.global memmove
.type memmove, @function
memmove:
    pushl %esi
    pushl %edi
    movl 12(%esp), %edi
    movl 16(%esp), %esi
    movl 20(%esp), %ecx
    movl %edi, %eax
    subl %esi, %eax
    cmpl %ecx, %eax
    ja 1f
    std
    leal -1(%edi, %ecx), %edi
    leal -1(%esi, %ecx), %esi
    rep
    movsb
    leal 1(%edi), %eax
    cld
    popl %edi
    popl %esi
    retl
1:
.hidden __memcpy_fwd
    jmp __memcpy_fwd
