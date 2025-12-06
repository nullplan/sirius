.syntax unified
.global __aeabi_memclr8
.global __aeabi_memclr4
.type __aeabi_memclr8, %function
.type __aeabi_memclr4, %function
__aeabi_memclr8:
__aeabi_memclr4:
    mov r2, 0
    b 1f
.size __aeabi_memclr8, . - __aeabi_memclr8
.size __aeabi_memclr4, . - __aeabi_memclr4

.global __aeabi_memset8
.global __aeabi_memset4
.type __aeabi_memset8, %function
.type __aeabi_memset4, %function
__aeabi_memset8:
__aeabi_memset4:
    and r2, 0xff
    lsl r3, r2, 8
    orr r2, r3
    lsl r3, r2, 16
    orr r2, r3
1:
    tst r1, 3
    bne __aeabi_memset
    add r3, r0, r1
    cmp r1, 120
    bhi 2f
    cmp r1, 0
    beq 1f
    str r2, [r0]
    str r2, [r3, -4]
    cmp r1, 9
    blo 1f
    str r2, [r0, 4]
    str r2, [r0, 8]
    str r2, [r3, -12]
    str r2, [r3, -8]
    cmp r1, 25
    blo 1f
    str r2, [r0, 12]
    str r2, [r0, 16]
    str r2, [r0, 20]
    str r2, [r0, 24]
    str r2, [r3, -28]
    str r2, [r3, -24]
    str r2, [r3, -20]
    str r2, [r3, -16]
    cmp r1, 57
    blo 1f
    str r2, [r0, 28]
    str r2, [r0, 32]
    str r2, [r0, 36]
    str r2, [r0, 40]
    str r2, [r0, 44]
    str r2, [r0, 48]
    str r2, [r0, 52]
    str r2, [r0, 56]
    str r2, [r3, -60]
    str r2, [r3, -56]
    str r2, [r3, -52]
    str r2, [r3, -48]
    str r2, [r3, -44]
    str r2, [r3, -40]
    str r2, [r3, -36]
    str r2, [r3, -32]
1:  bx lr
2:
    @ large case
    tst r0, 0x1f
    beq 1f
    str r2, [r0]
    str r2, [r0, 4]
    str r2, [r0, 8]
    str r2, [r0, 12]
    str r2, [r0, 16]
    str r2, [r0, 20]
    str r2, [r0, 24]
    str r2, [r0, 28]
    add r0, r0, 32
    and r0, -32
1:
    tst r3, 0x1f
    beq 1f
    str r2, [r3, -32]
    str r2, [r3, -28]
    str r2, [r3, -24]
    str r2, [r3, -20]
    str r2, [r3, -16]
    str r2, [r3, -12]
    str r2, [r3, -8]
    str r2, [r3, -4]
    and r3, -32
1:
    str r2, [r0]
    str r2, [r0, 4]
    str r2, [r0, 8]
    str r2, [r0, 12]
    str r2, [r0, 16]
    str r2, [r0, 20]
    str r2, [r0, 24]
    str r2, [r0, 28]
    add r0, r0, 32
    cmp r0, r3
    bne 1b
    bx lr
.size __aeabi_memset8, . - __aeabi_memset8
.size __aeabi_memset4, . - __aeabi_memset4

.global __aeabi_memclr
.type __aeabi_memclr, %function
__aeabi_memclr:
    mov r2, 0
    b 1f
.size __aeabi_memclr, . - __aeabi_memclr

.global __aeabi_memset
.type __aeabi_memset, %function
__aeabi_memset:
    and r2, 0xff
    lsl r3, r2, 8
    orr r2, r3
    lsl r3, r2, 16
    orr r2, r3
    add r3, r0, r1
    cmp r1, 121
    bhi 2f
    cmp r1, 0
    beq 1f
    strb r2, [r0]
    strb r2, [r3, -1]
    cmp r1, 3
    blo 1f
    strb r2, [r0, 1]
    strb r2, [r0, 2]
    strb r2, [r3, -3]
    strb r2, [r3, -2]
    cmp r1, 7
    blo 1f
    strb r2, [r0, 3]
    strb r2, [r3, -4]
    cmp r1, 9
    blo 1f
    add r0, r0, 4
    and r0, -4
    and r3, -4
    sub r1, r3, r0
    str r2, [r0]
    str r2, [r3, -4]
    cmp r1, 9
    blo 1f
    str r2, [r0, 4]
    str r2, [r0, 8]
    str r2, [r3, -12]
    str r2, [r3, -8]
    cmp r1, 25
    blo 1f
    str r2, [r0, 12]
    str r2, [r0, 16]
    str r2, [r0, 20]
    str r2, [r0, 24]
    str r2, [r3, -28]
    str r2, [r3, -24]
    str r2, [r3, -20]
    str r2, [r3, -16]
    cmp r1, 57
    blo 1f
    str r2, [r0, 28]
    str r2, [r0, 32]
    str r2, [r0, 36]
    str r2, [r0, 40]
    str r2, [r0, 44]
    str r2, [r0, 48]
    str r2, [r0, 52]
    str r2, [r0, 56]
    str r2, [r3, -60]
    str r2, [r3, -56]
    str r2, [r3, -52]
    str r2, [r3, -48]
    str r2, [r3, -44]
    str r2, [r3, -40]
    str r2, [r3, -36]
    str r2, [r3, -32]
1:  bx lr
2:  @ large case
    tst r0, 0x1f
    beq 2f
    tst r0, 0x03
    beq 1f
    strb r2, [r0]
    strb r2, [r0, 1]
    strb r2, [r0, 2]
    strb r2, [r0, 3]
    add r0, r0, 4
    and r0, -4
1:
    str r2, [r0]
    str r2, [r0, 4]
    str r2, [r0, 8]
    str r2, [r0, 12]
    str r2, [r0, 16]
    str r2, [r0, 20]
    str r2, [r0, 24]
    str r2, [r0, 28]
    add r0, r0, 32
    and r0, -32
2:
    tst r3, 0x1f
    beq 2f
    tst r3, 0x03
    beq 1f
    strb r2, [r3, -4]
    strb r2, [r3, -3]
    strb r2, [r3, -2]
    strb r2, [r3, -1]
    and r3, -4
1:
    strb r2, [r3, -32]
    strb r2, [r3, -28]
    strb r2, [r3, -24]
    strb r2, [r3, -20]
    strb r2, [r3, -16]
    strb r2, [r3, -12]
    strb r2, [r3, -8]
    strb r2, [r3, -4]
    and r3, -32
2:
    str r2, [r0]
    str r2, [r0, 4]
    str r2, [r0, 8]
    str r2, [r0, 12]
    str r2, [r0, 16]
    str r2, [r0, 20]
    str r2, [r0, 24]
    str r2, [r0, 28]
    add r0, r0, 32
    cmp r0, r3
    bne 2b
    bx lr
.size __aeabi_memset, . - __aeabi_memset
