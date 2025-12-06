.syntax unified
.global memcmp
.type memcmp, %function
memcmp:
    cmp r2, 0
    beq 3f
1:
    ldrb r3, [r0], 1
    ldrb ip, [r0], 1
    subs r3, r3, ip
    bne 2f
    subs r2, r2, 1
    bne 1b
2:
    mov r0, r3
    bx lr
3:
    mov r0, 0
    bx lr
.size memcmp, . - memcmp
