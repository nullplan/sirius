.syntax unified
.fpu vfp
.eabi_attribute 10, 0       @ pretend we don't use FPU here!
.eabi_attribute 27, 0       @ and we pass args the way implied from this
.global longjmp
.type longjmp, %function
longjmp:
    mov r2, r0
#if __ARM_ARCH < 6
    bl __aeabi_read_tp
#else
    mrc p15, 0, r0, c13, c0, 3
#endif
    ldr r0, [r0, -12]   @ hwcap
    ldmia r2!, {r4-r11}
    ldmia r2!, {r3, lr}
    mov sp, r3
#if __ARM_ARCH < 8
    tst r0, 0x20        @ FPA
    beq 1f
    ldc p2, cr4, [r2], 48
#endif
1:  tst r0, 0x40        @ VFP
    beq 1f
    vstmia r2!, {d8-d15}
#if __ARM_ARCH < 8
1:
    tst r0, 0x200       @ IWMMXT
    beq 1f
    ldcl p1, cr10, [r2], 8
    ldcl p1, cr11, [r2], 8
    ldcl p1, cr12, [r2], 8
    ldcl p1, cr13, [r2], 8
    ldcl p1, cr14, [r2], 8
    ldcl p1, cr15, [r2], 8
#endif
1:
    cmp r1, 0
    mov r0, r1
    moveq r0, 1
    bx lr
.size longjmp, . - longjmp
