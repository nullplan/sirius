.global memset
.type memset, @function
memset:
    pushl %esi
    pushl %edi
    movl 12(%esp), %edi
    movl 16(%esp), %eax
    movl 20(%esp), %ecx
    rep
    stosb
    movl 12(%esp), %eax
    popl %edi
    popl %esi
    retl
.size memset, . - memset
