.global memset
.type memset, @function
memset:
    cmpwi %r5, 0
    beqlr-
    addi %r6, %r3, -1
    mtctr %r5
1:
    stbu %r4, 1(%r6)
    bdnz 1b
    blr
.size memset, . - memset
