.global memcmp
.type memcmp, @function
memcmp:
    cbz x2, 2f
    mov x3, x0
1:
    ldrb w0, [x3], 1
    ldrb w4, [x1], 1
    subs w0, w0, w4
    b.ne 1f
    adds x2, x2, -1
    b.ne 1b
1:
    ret

2:
    mov w0, 0
    ret
.size memcmp, . - memcmp
