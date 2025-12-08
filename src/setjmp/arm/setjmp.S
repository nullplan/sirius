.syntax unified
.fpu vfp
.eabi_attribute 10, 0       @ ignore the code, we don't use FPU here!
.eabi_attribute 27, 0       @ and we pass args the way implied from this
.global setjmp
.type setjmp, %function
setjmp:
#if __ARM_ARCH < 6
    push {r0, lr}
    bl __aeabi_read_tp
    pop {r1, lr}
#else
    mov r1, r0
    mrc p15, 0, r0, c13, c0, 3
#endif
    ldr r0, [r0, -12]   @ hwcap
    stmia r1!, {r4-r11}
    mov r2, sp
    stmia r1!, {r2, lr}
    /* apparently, LLVM doesn't like old coproc instructions when compiling for armv8. */
#if __ARM_ARCH < 8
    tst r0, 0x20        @ FPA
    beq 1f
    stc p2, cr4, [r1], 48
#endif
1:  tst r0, 0x40        @ VFP
    beq 1f
    vstmia r1!, {d8-d15}
#if __ARM_ARCH < 8
1:  tst r0, 0x200       @ IWMMXT
    beq 1f
    stcl p1, cr10, [r1], 8
    stcl p1, cr11, [r1], 8
    stcl p1, cr12, [r1], 8
    stcl p1, cr13, [r1], 8
    stcl p1, cr14, [r1], 8
    stcl p1, cr15, [r1], 8
#endif
1:
    mov r0, 0
    bx lr
.size setjmp, . - setjmp
