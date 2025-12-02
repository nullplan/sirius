.global memmove
.type memmove, %function
memmove:
    push {r0, lr}
#if __ARM_ARCH < 5
    mov lr, pc
    bl __aeabi_memmove
    pop {r0, lr}
    bx lr
#else
    blx __aeabi_memmove
    pop {r0, pc}
#endif
