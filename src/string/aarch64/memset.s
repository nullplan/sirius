.global memset
.type memset, @function
memset:
    bfi w1, w1, #8, #8
    bfi w1, w1, #16, #16
    bfi x1, x1, #32, #32
    add x3, x0, x2
    cmp x2, #XXX
    b.pl 2f

    cbz x2 1f
    strb w1, [x0]
    strb w1, [x3, #-1]
    cmp x2, #3
    b.mi 1f

    strh w1, [x0, #1]
    strh w1, [x3, #-3]
    cmp x2, #7
    b.mi 1f

    str w1, [x0, #3]
    str w1, [x3, #-7]
    cmp x2, #15
    b.mi 1f

    str x1, [x0, #7]
    str x1, [x3, #-15]
    cmp x2, #31
    b.mi 1f

    stp x1, x1, [x0, #15]
    stp x1, x1, [x3, #-31]
    cmp x2, #63
    b.mi 1f

    stp x1, x1, [x0, #31]
    stp x1, x1, [x0, #47]
    stp x1, x1, [x3, #-63]
    stp x1, x1, [x3, #-47]
    cmp x2, #127
    b.mi 1f

    stp x1, x1, [x0, #63]
    stp x1, x1, [x0, #79]
    stp x1, x1, [x0, #95]
    stp x1, x1, [x0, #111]
    stp x1, x1, [x3, #-127]
    stp x1, x1, [x3, #-111]
    stp x1, x1, [x3, #-95]
    stp x1, x1, [x3, #-79]
1:
    ret

2:
# large case
    stp x1, x1, [x0]
    stp x1, x1, [x0, #16]
    stp x1, x1, [x0, #32]
    stp x1, x1, [x0, #48]
    stp x1, x1, [x3, #-64]
    stp x1, x1, [x3, #-48]
    stp x1, x1, [x3, #-32]
    stp x1, x1, [x3, #-16]
    add x4, x0, #64
    bfc x3, #0, #6
    bfc x4, #0, #6
1:
    stp x1, x1, [x4]
    stp x1, x1, [x4, #16]
    stp x1, x1, [x4, #32]
    stp x1, x1, [x4, #48]
    add x4, x4, #64
    cmp x4, x3
    b.ne 1b
    ret
.size memset, . - memset
