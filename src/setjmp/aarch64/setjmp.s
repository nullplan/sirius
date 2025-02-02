.global setjmp
.type setjmp, @function
.global __setjmp
.hidden __setjmp
setjmp:
__setjmp:
    mov x1, sp
    stp x1, x19, [x0]
    stp x20, x21, [x0, #16]
    stp x22, x23, [x0, #32]
    stp x24, x25, [x0, #48]
    stp x26, x27, [x0, #64]
    stp x28, x29, [x0, #80]
    str x30, [x0, #96]
    stp d8, d9, [x0, #112]
    stp d10, d11, [x0, #128]
    stp d12, d13, [x0, #144]
    stp d14, d15, [x0, #160]
    mov x0, #0
    ret
.size setjmp, . - setjmp
