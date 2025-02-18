.global memset
.type memset, @function
memset:
    add %r6, %r3, %r5
    rlwimi %r4, %r4, 8, 16, 23
    rlwimi %r4, %r4, 16, 0, 15
    # last aligned stage handles up to 120 bytes,
    # and worst-case length reduction before that is 1
    # so the small algo can handle all sizes up to 121 under all circumstances.
    cmplwi %r5, 121
    bgt 2f
    cmplwi %r5, 0
    beqlr-
    stb %r4, 0(%r3)
    stb %r4, -1(%r6)
    cmplwi %r5, 2
    bnglr
    stb %r4, 1(%r3)
    stb %r4, 2(%r3)
    stb %r4, -3(%r6)
    stb %r4, -2(%r6)
    cmplwi %r5, 6
    bnglr
    stb %r4, 3(%r3)
    stb %r4, -4(%r6)
    cmplwi %r5, 8
    bnglr
    # %r5 >= 9
    addi %r7, %r3, 4
    clrrwi %r6, %r6, 2
    clrrwi %r7, %r7, 2
    sub %r5, %r6, %r7
    # r5 is now reduced by at most 7, and is now divisible by 4
    # so %r5 >= 4
    stw %r4, 0(%r7)
    stw %r4, -4(%r6)
    cmplwi %r5, 8
    bnglr
    stw %r4, 4(%r7)
    stw %r4, 8(%r7)
    stw %r4, -12(%r6)
    stw %r4, -8(%r6)
    cmplwi %r5, 24
    bnglr
    stw %r4, 12(%r7)
    stw %r4, 16(%r7)
    stw %r4, 20(%r7)
    stw %r4, 24(%r7)
    stw %r4, -28(%r6)
    stw %r4, -24(%r6)
    stw %r4, -20(%r6)
    stw %r4, -16(%r6)
    cmplwi %r5, 56
    bnglr
    stw %r4, 28(%r7)
    stw %r4, 32(%r7)
    stw %r4, 36(%r7)
    stw %r4, 40(%r7)
    stw %r4, 44(%r7)
    stw %r4, 48(%r7)
    stw %r4, 52(%r7)
    stw %r4, 56(%r7)
    stw %r4, -60(%r6)
    stw %r4, -56(%r6)
    stw %r4, -52(%r6)
    stw %r4, -48(%r6)
    stw %r4, -44(%r6)
    stw %r4, -40(%r6)
    stw %r4, -36(%r6)
    stw %r4, -32(%r6)
    blr

2:
# large subcase
    stb %r4, 0(%r3)
    stb %r4, 1(%r3)
    stb %r4, 2(%r3)
    stb %r4, 3(%r3)
    stb %r4, -4(%r6)
    stb %r4, -3(%r6)
    stb %r4, -2(%r6)
    stb %r4, -1(%r6)
    addi %r7, %r3, 4
    clrrwi %r6, %r6, 2
    clrrwi %r7, %r7, 2
    stw %r4, 0(%r7)
    stw %r4, 4(%r7)
    stw %r4, 8(%r7)
    stw %r4, 12(%r7)
    stw %r4, 16(%r7)
    stw %r4, 20(%r7)
    stw %r4, 24(%r7)
    stw %r4, 28(%r7)
    stw %r4, -32(%r6)
    stw %r4, -28(%r6)
    stw %r4, -24(%r6)
    stw %r4, -20(%r6)
    stw %r4, -16(%r6)
    stw %r4, -12(%r6)
    stw %r4, -8(%r6)
    stw %r4, -4(%r6)
    addi %r7, %r7, 32
    clrrwi %r6, %r6, 5
    clrrwi %r7, %r7, 5
1:
    stw %r4, 0(%r7)
    stw %r4, 4(%r7)
    stw %r4, 8(%r7)
    stw %r4, 12(%r7)
    stw %r4, 16(%r7)
    stw %r4, 20(%r7)
    stw %r4, 24(%r7)
    stw %r4, 28(%r7)
    addi %r7, %r7, 32
    cmplw %r7, %r6
    bne 1b
    blr
.size memset, . - memset
