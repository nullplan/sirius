.global memcpy
.type memcpy, @function
.global __memcpy_fwd
.hidden __memcpy_fwd
memcpy:
__memcpy_fwd:
    movl %edx, %ecx
    movl %edi, %eax
    rep
    movsb
    retq
.size memcpy, . - memcpy
