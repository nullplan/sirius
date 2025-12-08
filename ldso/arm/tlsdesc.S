@ the argument is in r0, and I return the offset relative to $tp
.syntax unified
.global __tlsdesc_static
.hidden __tlsdesc_static
.type __tlsdesc_static, %function
__tlsdesc_static:
    ldr r0, [r0]
    bx lr
.size __tlsdesc_static, . - __tlsdesc_static

.global __tlsdesc_dynamic
.hidden __tlsdesc_dynamic
.type __tlsdesc_dynamic, %function
__tlsdesc_dynamic:
    push {r2, lr}
    mov r1, r0
#if __ARM_ARCH >= 6
    mrc p15, 0, r0, cr13, cr0, 3
#else
    bl __aeabi_read_tp
#endif
    ldr r1, [r1]        @ r1 = {tlsmod, tlsoff}
    ldr r2, [r1]        @ r2 = tlsmod
    ldr r1, [r1, 4]     @ r1 = tlsoff
    sub r1, r0          @ r1 = tlsoff - $tp
    ldr r0, [r0, -4]    @ r0 = dtv
    ldr r0, [r0, r2, lsl 2] @ r0 = dtv[tlsmod]
    add r0, r1          @ r1 = dtv[tlsmod] + tlsoff - $tp
#if __ARM_ARCH < 5
    pop {r2, lr}
    bx lr
#else
    pop {r2, pc}
#endif


.size __tlsdesc_dynamic, . - __tlsdesc_static

