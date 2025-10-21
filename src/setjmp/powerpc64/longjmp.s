.machine altivec
.global longjmp
.type longjmp, @function
longjmp:
    ld %r2, 0(%r3)
    ld %r1, 8(%r3)
    ld %r13, 16(%r3)
    ld %r14, 24(%r3)
    ld %r15, 32(%r3)
    ld %r16, 40(%r3)
    ld %r17, 48(%r3)
    ld %r18, 56(%r3)
    ld %r19, 64(%r3)
    ld %r20, 72(%r3)
    ld %r21, 80(%r3)
    ld %r22, 88(%r3)
    ld %r23, 96(%r3)
    ld %r24, 104(%r3)
    ld %r25, 112(%r3)
    ld %r26, 120(%r3)
    ld %r27, 128(%r3)
    ld %r28, 136(%r3)
    ld %r29, 144(%r3)
    ld %r30, 152(%r3)
    ld %r31, 160(%r3)
    ld %r5, 168(%r3)
    ld %r6, 176(%r3)
    lfd %f14, 184(%r3)
    lfd %f15, 192(%r3)
    lfd %f16, 200(%r3)
    lfd %f17, 208(%r3)
    lfd %f18, 216(%r3)
    lfd %f19, 224(%r3)
    lfd %f20, 232(%r3)
    lfd %f21, 240(%r3)
    lfd %f22, 248(%r3)
    lfd %f23, 256(%r3)
    lfd %f24, 264(%r3)
    lfd %f25, 272(%r3)
    lfd %f26, 280(%r3)
    lfd %f27, 288(%r3)
    lfd %f28, 296(%r3)
    lfd %f29, 304(%r3)
    lfd %f30, 312(%r3)
    lfd %f31, 320(%r3)
    ld %r7, -0x7018(%r13)
    andis. %r7, %r7, 0x1000
    beq 1f
    li %r7, 336
    lvx %v20, %r7, %r3
    li %r7, 352
    lvx %v21, %r7, %r3
    li %r7, 368
    lvx %v22, %r7, %r3
    li %r7, 384
    lvx %v23, %r7, %r3
    li %r7, 400
    lvx %v24, %r7, %r3
    li %r7, 416
    lvx %v25, %r7, %r3
    li %r7, 432
    lvx %v26, %r7, %r3
    li %r7, 448
    lvx %v27, %r7, %r3
    li %r7, 464
    lvx %v28, %r7, %r3
    li %r7, 480
    lvx %v29, %r7, %r3
    li %r7, 496
    lvx %v30, %r7, %r3
    li %r7, 512
    lvx %v31, %r7, %r3
1:
    mtctr %r5
    mtcrf 255, %r6
    std %r2, 24(%r1)
    cntlzw %r3, %r4
    srwi %r3, %r3, 5
    or %r3, %r3, %r4
    bctr
.size longjmp, . - longjmp
