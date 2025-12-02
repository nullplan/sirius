.syntax unified
.global memcpy
.type memcpy, %function
memcpy:
    push {r0, lr}
#if __ARM_ARCH < 5
    mov lr, pc
    bl __aeabi_memcpy
    pop {r0, lr}
    bx lr
#else
    blx __aeabi_memcpy
    pop {r0, pc}
#endif
.size memcpy, . - memcpy
