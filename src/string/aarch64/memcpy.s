.global memcpy
.type memcpy, @function
memcpy:
    cbz x2, 2f
.global __memcpy_fwd
.hidden __memcpy_fwd
    mov x3, x0
1:
    ldrb w4, [x1], #1 !
    strb w3, [x3], #1 !
    adds x2, x2, -1
    b.ne 1b
2:
    ret

.size memcpy, . - memcpy
