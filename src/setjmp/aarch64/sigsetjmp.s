.global sigsetjmp
.type sigsetjmp, @function
sigsetjmp:
    cbz w1, 1f
    stp x19, lr, [x0, 184]
    mov x19, x0
.hidden __setjmp
    bl __setjmp
    mov w1, w0
    mov x0, x19
    ldp x19, lr, [x0, 184]
.hidden __sigsetjmp_tail
    b __sigsetjmp_tail
1:
    b __setjmp
