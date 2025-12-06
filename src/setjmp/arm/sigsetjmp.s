.syntax unified
.global sigsetjmp
.type sigsetjmp, %function
sigsetjmp:
    cmp r1, 0
    beq 3f
    str r4, [r0, 26*8]
    str lr, [r0, 26*8+4]
    mov r4, r0
    bl setjmp
    mov r1, r0
    mov r0, r4
    ldr r4, [r0, 26*8]
    ldr lr, [r0, 26*8+4]
.hidden __sigsetjmp_tail
    b __sigsetjmp_tail
3: b setjmp
.size sigsetjmp, . - sigsetjmp
