.global memmove
.type memmove, @function
memmove:
    cmplwi %r5, 0
    beqlr-
    sub %r0, %r3, %r4
    cmpw %r0, %r5
    bnl 2f
    add %r3, %r3, %r5
    add %r4, %r4, %r5
    mtctr %r5
1:
    lbzu %r7, -1(%r4)
    stbu %r7, -1(%r3)
    bdnz 1b
    blr
2:
.hidden __memcpy_fwd
    b __memcpy_fwd
