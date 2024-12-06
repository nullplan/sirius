.global memcmp
.type memcmp, @function
memcmp:
    addi %r6, %r3, -1
    addi %r4, %r4, -1
    li %r3, 0
    cmpwi %r5, 0
    beqlr-
    mtctr %r5
1:
    lbzu %r3, 1(%r6)
    lbzu %r7, 1(%r4)
    sub. %r3, %r3, %r7
    bdnzt eq, 1b
    blr
.size memcmp, . - memcmp
