.global memcpy
.type memcpy, @function
.global __memcpy_fwd
.hidden __memcpy_fwd
memcpy:
__memcpy_fwd:
    movq %rdx, %rcx
    movq %rdi, %rax
    rep
    movsb
    retq
.size memcpy, . - memcpy
