.global setjmp
.type setjmp, @function
.global __setjmp
.hidden __setjmp
setjmp:
__setjmp:
    mflr %r0
    stw %r1, 0(%r3)
    stw %r2, 4(%r3)
    stw %r13, 8(%r3)
    stw %r14, 12(%r3)
    stw %r15, 16(%r3)
    stw %r16, 20(%r3)
    stw %r17, 24(%r3)
    stw %r18, 28(%r3)
    stw %r19, 32(%r3)
    stw %r20, 36(%r3)
    stw %r21, 40(%r3)
    stw %r22, 44(%r3)
    stw %r23, 48(%r3)
    stw %r24, 52(%r3)
    stw %r25, 56(%r3)
    stw %r26, 60(%r3)
    stw %r27, 64(%r3)
    stw %r28, 68(%r3)
    stw %r29, 72(%r3)
    stw %r30, 76(%r3)
    stw %r31, 80(%r3)
    stw %r0, 84(%r3)
#ifndef __NO_FPRS__
    stfd %f14, 96(%r3)
    stfd %f15, 104(%r3)
    stfd %f16, 112(%r3)
    stfd %f17, 120(%r3)
    stfd %f18, 128(%r3)
    stfd %f19, 136(%r3)
    stfd %f20, 144(%r3)
    stfd %f21, 152(%r3)
    stfd %f22, 160(%r3)
    stfd %f23, 168(%r3)
    stfd %f24, 176(%r3)
    stfd %f25, 184(%r3)
    stfd %f26, 192(%r3)
    stfd %f27, 200(%r3)
    stfd %f28, 208(%r3)
    stfd %f29, 216(%r3)
    stfd %f30, 224(%r3)
    stfd %f31, 232(%r3)
#endif
    lwz %r4, -0x700C(%r2)
    andis. %r4, %r4, 0x1000
    beq 1f
    mfspr %r4, 256
    li %r5, 240
    stvx %v20, %r5, %r3
    li %r5, 256
    stvx %v21, %r5, %r3
    li %r5, 272
    stvx %v22, %r5, %r3
    li %r5, 288
    stvx %v23, %r5, %r3
    li %r5, 304
    stvx %v24, %r5, %r3
    li %r5, 320
    stvx %v25, %r5, %r3
    li %r5, 336
    stvx %v26, %r5, %r3
    li %r5, 352
    stvx %v27, %r5, %r3
    li %r5, 368
    stvx %v28, %r5, %r3
    li %r5, 384
    stvx %v29, %r5, %r3
    li %r5, 400
    stvx %v30, %r5, %r3
    li %r5, 416
    stvx %v31, %r5, %r3
    stw %r4, 92(%r3)
1:
    li %r3, 0
    blr
.size setjmp, . - setjmp
