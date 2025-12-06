.syntax unified
.global memcpy
.type memcpy, %function
memcpy:
    push {r0, lr}
#if __ARM_ARCH < 5
    bl __aeabi_memcpy
    pop {r0, r1}
    bx r1
#else
    blx __aeabi_memcpy
    pop {r0, pc}
#endif
.size memcpy, . - memcpy
