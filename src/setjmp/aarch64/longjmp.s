.global longjmp
.type longjmp, @function
longjmp:
    ldp x2, x19, [x0]
    ldp x20, x21, [x0, 16]
    ldp x22, x23, [x0, 32]
    ldp x24, x25, [x0, 48]
    ldp x26, x27, [x0, 64]
    ldp x28, x29, [x0, 80]
    ldr x3, [x0, 96]
    ldp d8, d9, [x0, 112]
    ldp d10, d11, [x0, 128]
    ldp d12, d13, [x0, 144]
    ldp d14, d15, [x0, 160]
    mov sp, x2
    clz w0, w1
    lsr w0, w0, 5
    orr w0, w0, w1
    br x3
.size longjmp, . - longjmp
