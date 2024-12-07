.global memcpy
.type memcpy, @function
.global __memcpy_fwd
.hidden __memcpy_fwd
memcpy:
    pushl %esi
    pushl %edi
    movl 12(%esp), %edi
    movl 16(%esp), %esi
    movl 20(%esp), %ecx
__memcpy_fwd:
    rep
    movsb
    movl 12(%esp), %eax
    popl %edi
    popl %esi
    retl
.size memcpy, . - memcpy
