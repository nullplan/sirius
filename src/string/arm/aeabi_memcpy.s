.syntax unified
.global __aeabi_memcpy
.global __aeabi_memcpy4
.global __aeabi_memcpy8
.type __aeabi_memcpy, %function
.type __aeabi_memcpy4, %function
.type __aeabi_memcpy8, %function
.global __memcpy_fwd4
.type __memcpy_fwd4, %function
.hidden __memcpy_fwd4
__memcpy_fwd4:
__aeabi_memcpy4:
__aeabi_memcpy8:
    b 2f
1:
    ldr r3, [r1], 4
    str r3, [r1], 4
    sub r2, r2, 4
2:
    cmp r2, 3
    bhi 1b

.global __memcpy_fwd
.type __memcpy_fwd, %function
.hidden __memcpy_fwd
__memcpy_fwd:
__aeabi_memcpy:
    cmp r2, 0
    beq 2f
1:
    ldrb r3, [r1], 1
    strb r3, [r0], 1
    subs r2, r2, 1
    bne 1b
2:  bx lr
.size __aeabi_memcpy, . - __aeabi_memcpy
.size __aeabi_memcpy4, . - __aeabi_memcpy4
.size __aeabi_memcpy8, . - __aeabi_memcpy8
