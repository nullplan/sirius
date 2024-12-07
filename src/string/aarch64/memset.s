.global memset
.type memset, @function
memset:
    cbz x2, 2f
    mov x3, x0
1:
    strb w1, [x3], #1!
    adds x2, x2, -1
    b.ne 1b
2:
    ret
.size memset, . - memset
