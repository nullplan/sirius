.global memset
.type memset, @function
memset:
    rlwimi %r4, %r4, 8, 16, 23
    rlwimi %r4, %r4, 16, 0, 15
    rldimi %r4, %r4, 32, 0
    add %r6, %r3, %r5
    cmpldi %r5, 241
    bgt 2f

    cmpldi %r5, 0
    beqlr-

    stb %r4, 0(%r3)
    stb %r4, -1(%r6)
    cmpldi %r5, 2
    bnglr

    stb %r4, 1(%r3)
    stb %r4, 2(%r3)
    stb %r4, -3(%r6)
    stb %r4, -2(%r6)
    cmpldi %r5, 6
    bnglr

    stb %r4, 3(%r3)
    stb %r4, 4(%r3)
    stb %r4, 5(%r3)
    stb %r4, 6(%r3)
    stb %r4, -7(%r6)
    stb %r4, -6(%r6)
    stb %r4, -5(%r6)
    stb %r4, -4(%r6)

    cmpldi %r5, 14
    bnglr

    stb %r4, 7(%r3)
    stb %r4, -8(%r6)

    cmpldi %r5, 16
    bnglr

    addi %r7, %r3, 8
    clrrdi %r6, %r6, 3
    clrrdi %r7, %r7, 3
    sub %r5, %r6, %r7

    std %r4, 0(%r7)
    std %r4, -8(%r6)
    cmpldi %r5, 16
    bnglr

    std %r4, 8(%r7)
    std %r4, 16(%r7)
    std %r4, -24(%r7)
    std %r4, -16(%r7)
    cmpldi %r5, 48
    bnglr

    std %r4, 24(%r7)
    std %r4, 32(%r7)
    std %r4, 40(%r7)
    std %r4, 48(%r7)
    std %r4, -56(%r6)
    std %r4, -48(%r6)
    std %r4, -40(%r6)
    std %r4, -32(%r6)
    cmpldi %r5, 112
    bnglr

    std %r4, 56(%r7)
    std %r4, 64(%r7)
    std %r4, 72(%r7)
    std %r4, 80(%r7)
    std %r4, 88(%r7)
    std %r4, 96(%r7)
    std %r4, 104(%r7)
    std %r4, 112(%r7)
    std %r4, -120(%r6)
    std %r4, -112(%r6)
    std %r4, -104(%r6)
    std %r4, -96(%r6)
    std %r4, -88(%r6)
    std %r4, -80(%r6)
    std %r4, -72(%r6)
    std %r4, -64(%r6)
    blr

2:
# large case
    stb %r4, 0(%r3)
    stb %r4, 1(%r3)
    stb %r4, 2(%r3)
    stb %r4, 3(%r3)
    stb %r4, 4(%r3)
    stb %r4, 5(%r3)
    stb %r4, 6(%r3)
    stb %r4, 7(%r3)
    stb %r4, -8(%r6)
    stb %r4, -7(%r6)
    stb %r4, -6(%r6)
    stb %r4, -5(%r6)
    stb %r4, -4(%r6)
    stb %r4, -3(%r6)
    stb %r4, -2(%r6)
    stb %r4, -1(%r6)
    addi %r7, %r3, 8
    clrrdi %r6, %r6, 3
    clrrdi %r7, %r7, 3
    std %r4, 0(%r7)
    std %r4, 8(%r7)
    std %r4, 16(%r7)
    std %r4, 24(%r7)
    std %r4, 32(%r7)
    std %r4, 40(%r7)
    std %r4, 48(%r7)
    std %r4, 56(%r7)
    std %r4, -64(%r6)
    std %r4, -56(%r6)
    std %r4, -48(%r6)
    std %r4, -40(%r6)
    std %r4, -32(%r6)
    std %r4, -24(%r6)
    std %r4, -16(%r6)
    std %r4, -8(%r6)

    addi %r7, %r7, 64
    clrrdi %r6, %r6, 6
    clrrdi %r7, %r7, 6
1:
    std %r4, 0(%r7)
    std %r4, 8(%r7)
    std %r4, 16(%r7)
    std %r4, 24(%r7)
    std %r4, 32(%r7)
    std %r4, 40(%r7)
    std %r4, 48(%r7)
    std %r4, 56(%r7)
    addi %r7, %r7, 64
    cmpld %r7, %r6
    bne 1b
    blr
.size memset, . - memset
