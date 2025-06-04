.global __setjmp
.hidden __setjmp
__setjmp:
    std %r5, 0(%r3)
    b 1f
.global setjmp
.type setjmp, @function
setjmp:
    std %r2, 0(%r3)
1:
    std %r1, 8(%r3)
    mflr %r0
    mfcr %r4
    std %r13, 16(%r3)
    std %r14, 24(%r3)
    std %r15, 32(%r3)
    std %r16, 40(%r3)
    std %r17, 48(%r3)
    std %r18, 56(%r3)
    std %r19, 64(%r3)
    std %r20, 72(%r3)
    std %r21, 80(%r3)
    std %r22, 88(%r3)
    std %r23, 96(%r3)
    std %r24, 104(%r3)
    std %r25, 112(%r3)
    std %r26, 120(%r3)
    std %r27, 128(%r3)
    std %r28, 136(%r3)
    std %r29, 144(%r3)
    std %r30, 152(%r3)
    std %r31, 160(%r3)
    std %r0, 168(%r3)
    std %r4, 176(%r3)
    stfd %f14, 184(%r3)
    stfd %f15, 192(%r3)
    stfd %f16, 200(%r3)
    stfd %f17, 208(%r3)
    stfd %f18, 216(%r3)
    stfd %f19, 224(%r3)
    stfd %f20, 232(%r3)
    stfd %f21, 240(%r3)
    stfd %f22, 248(%r3)
    stfd %f23, 256(%r3)
    stfd %f24, 264(%r3)
    stfd %f25, 272(%r3)
    stfd %f26, 280(%r3)
    stfd %f27, 288(%r3)
    stfd %f28, 296(%r3)
    stfd %f29, 304(%r3)
    stfd %f30, 312(%r3)
    stfd %f31, 320(%r3)
    ld %r4, -0x7018(%r13)
    andis. %r4, %r4, 0x1000
    beq 1f
    li %r4, 336
    stvx %v20, %r4, %r3
    li %r4, 352
    stvx %v21, %r4, %r3
    li %r4, 368
    stvx %v22, %r4, %r3
    li %r4, 384
    stvx %v23, %r4, %r3
    li %r4, 400
    stvx %v24, %r4, %r3
    li %r4, 416
    stvx %v25, %r4, %r3
    li %r4, 432
    stvx %v26, %r4, %r3
    li %r4, 448
    stvx %v27, %r4, %r3
    li %r4, 464
    stvx %v28, %r4, %r3
    li %r4, 480
    stvx %v29, %r4, %r3
    li %r4, 496
    stvx %v30, %r4, %r3
    li %r4, 512
    stvx %v31, %r4, %r3
1:
    li %r3, 0
    blr
.size setjmp, . - setjmp
