.global sigsetjmp
.type sigsetjmp, @function
sigsetjmp:
    cmplwi %r4, 0
    beq 1f
    mflr %r4
    stw %r14, 440(%r3)
    stw %r4, 444(%r3)
    mr %r14, %r3
.hidden __setjmp
    bl __setjmp
    mr %r4, %r3
    mr %r3, %r14
    lwz %r14, 440(%r3)
    lwz %r5, 444(%r3)
    mtlr %r5
.hidden __sigsetjmp_tail
    b __sigsetjmp_tail

1:
    b __setjmp
