.syntax unified
#if __ARM_ARCH >= 6 && !defined __thumb2__
.arm
#endif
.global __aeabi_read_tp
.type __aeabi_read_tp, %function
__aeabi_read_tp:
#if __ARM_ARCH >= 6
    mrc p15, 0, r0, c13, c0, 3
    bx lr
#else
    ldr r0, 1f
    add r0, pc, r0
    ldr r0, [r0]
2:  bx r0
.balign 4
.hidden __a_gettp_ptr
1: .word __a_gettp_ptr - 2b
#endif
.size __aeabi_read_tp, . - __aeabi_read_tp
