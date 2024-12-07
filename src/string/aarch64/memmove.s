.global memmove
.type memmove, @function
memmove:
    cbz x2, 2f
    sub x3, x0, x1
    cmp x3, x2
    b.lt 3f
    add x0, x0, x2
    add x1, x1, x2
1:
    ldrb w3, [x1, #-1]!
    strb w3, [x0, #-1]!
    adds x2, x2, -1
    b.ne 1b
2:
    ret
3:
.hidden __memcpy_fwd
    b __memcpy_fwd
.size memmove, . - memmove
