.syntax unified
.global memset
.type memset, %function
memset:
    push {r0, lr}
    eors r1, r2
    eors r2, r1
    eors r1, r2
#if __ARM_ARCH < 5
    mov lr, pc
    bl __aeabi_memset
    pop {r0, lr}
    bx lr
#else
    blx __aeabi_memset
    pop {r0, pc}
#endif
.size memset, . - memset
