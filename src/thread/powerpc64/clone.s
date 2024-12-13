.global __clone
.hidden __clone
.type __clone, @function
__clone:
# int __clone(int (*fn)(void *), void *stack, int flg, void *arg, int *ptid, uintptr_t tls, int *ctid);
#                  r3                r4          r5          r6          r7        r8            r9
# sys_clone(flags, stack, ptid, tls, ctid)
#            r3      r4     r5   r6    r7
    stdu %r1, -48(%r1)
    std %r31, 40(%r1)
    cmpldi %r4, 0
    beq 1f
    li %r0, 0
    clrrdi %r4, %r4, 4
    stdu %r0, -48(%r4)
    std %r3, 32(%r4)
    std %r6, 40(%r4)
1:
    mr %r31, %r4
    li %r0, 120
    mr %r3, %r5
    mr %r5, %r7
    mr %r6, %r8
    mr %r7, %r9
    sc
    bns 1f
    neg %r3, %r3
1:
    cmpwi %r3, 0
    bne 1f
    cmpldi %r31, 0
    bne 2f
1:
    ld %r31, 40(%r1)
    addi %r1, %r1, 48
    blr
.size __clone, . - __clone

2:
    ld %r4, 32(%r1)
    ld %r3, 40(%r1)
    mtctr %r4
    bctrl
    mr %r31, %r3
1:
    li %r0, 1
    mr %r3, %r31
    sc
    b 1b
