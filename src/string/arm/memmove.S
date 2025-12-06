.syntax unified
#ifdef __thumb__
.thumb
#endif
.global memmove
.type memmove, %function
memmove:
    push {r0, lr}
#if __ARM_ARCH < 5
    bl __aeabi_memmove
    pop {r0, r1}
    bx r1
#else
    blx __aeabi_memmove
    pop {r0, pc}
#endif
