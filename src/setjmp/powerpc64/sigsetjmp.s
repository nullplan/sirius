.global sigsetjmp
.type sigsetjmp, @function
sigsetjmp:
    addis %r2, %r12, .TOC. - sigsetjmp@ha
    addi %r2, %r2, .TOC. - sigsetjmp@l
    ld %r5, 24(%r1)
    b 1f
    .localentry sigsetjmp, . - sigsetjmp
    mr %r5, %r2
1:
    cmpwi %r4, 0
    beq 1f
    mflr %r6
    std %r14, 536(%r3)
    std %r6, 544(%r3)
    std %r2, 552(%r3)
    mr %r14, %r3
.hidden __setjmp
    bl __setjmp
    mr %r4, %r3
    mr %r3, %r14
    ld %r14, 536(%r3)
    ld %r6, 544(%r3)
    ld %r2, 552(%r3)
    mtlr %r6
.hidden __sigsetjmp_tail
    b __sigsetjmp_tail

1:
    b __setjmp

