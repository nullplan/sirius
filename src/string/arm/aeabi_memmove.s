.syntax unified
.global __aeabi_memmove4
.global __aeabi_memmove8
.type __aeabi_memmove4, %function
.type __aeabi_memmove8, %function
__aeabi_memmove4:
__aeabi_memmove8:
    teq r0, r1
    cmpne r2, 0
    beq 2f

    tst r2, 3
    bne __aeabi_memmove
    @ per usual, copy backward if dst ∈ [src, src + n] ↔ dst - src <U n
    subs r3, r0, r1
    cmp r2, r3
    bhi 3f
1:
    subs r2, r2, 4
    ldr r3, [r1, r2]
    str r3, [r0, r2]
    bne 1b
2:
    bx lr
3:
.hidden __memcpy_fwd4
    bl __memcpy_fwd4
.size __aeabi_memmove4, . - __aeabi_memmove4
.size __aeabi_memmove8, . - __aeabi_memmove8

.global __aeabi_memmove
.type __aeabi_memmove, %function
__aeabi_memmove:
    teq r0, r1
    cmpne r2, 0
    beq 2f
    subs r3, r0, r1
    cmp r2, r3
    bhi 3f
1:
    subs r2, r2, 1
    ldrb r3, [r1, r2]
    strb r3, [r0, r2]
    bne 1b
2:
    bx lr
3:
.hidden __memcpy_fwd
    bl __memcpy_fwd
.size __aeabi_memmove, . - __aeabi_memmove
