.global memcpy
.type memcpy, @function
memcpy:
    cmplwi %r5, 0
    beqlr-
.global __memcpy_fwd
.hidden __memcpy_fwd
__memcpy_fwd:
    mtctr %r5
    addi %r6, %r3, -1
    addi %r4, %r4, -1
1:
    lbzu %r7, 1(%r4)
    stbu %r7, 1(%r6)
    bdnz 1b
    blr
.size memcpy, . - memcpy
