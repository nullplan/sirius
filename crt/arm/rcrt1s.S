/* relevant parts of elf.h */
#define AT_PHDR   3
#define AT_PHENT  4
#define AT_PHNUM  5
#define AT_BASE   7
#define DT_RELA     7
#define DT_RELASZ   8
#define DT_REL  17
#define DT_RELSZ    18
#define DT_RELRSZ	35
#define DT_RELR		36
#define PT_DYNAMIC  2
#define R_ARM_RELATIVE 23
#define AUX_CNT (AT_BASE + 1)
#define DYN_CNT (DT_RELR + 1)
.syntax unified
1: .word _DYNAMIC-2f
.global _start
.type _start, %function
_start:
    mov r4, sp
    and r0, r4, -8
    mov sp, r0
    mov r0, AUX_CNT + DYN_CNT
    mov r1, 0
2:
    str r1, [sp, -4]!
    subs r0, 1
    bne 2b
    ldr r5, 1b
    add r5, pc, r5
    ldr r0, [r4]
2:  add r0, r4, r0, lsl 2
    add r0, r0, 4
1:  ldr r3, [r0], 4
    cmp r3, 0
    bne 1b
    mov r1, sp
    mov r2, AUX_CNT
    bl decode_vec
    mov r0, r5
    add r1, sp, AUX_CNT*4
    mov r2, DYN_CNT
    bl decode_vec
    ldr r0, [sp, AT_BASE*4]
    cmp r0, 0
    bne 1f
    ldr r0, [sp, AT_PHDR*4]
    ldr r1, [sp, AT_PHNUM*4]
    ldr r2, [sp, AT_PHENT*4]
    bl find_base
    mov r6, r0

    ldr r1, [sp, (AUX_CNT+DT_RELSZ)*4]
    cmp r1, 0
    beq 1f
    ldr r0, [sp, (AUX_CNT+DT_REL)*4]
    add r0, r6, r0
    bl do_rel
1:

    ldr r1, [sp, (AUX_CNT+DT_RELASZ)*4]
    cmp r1, 0
    beq 1f
    ldr r0, [sp, (AUX_CNT+DT_RELA)*4]
    add r0, r6, r0
    bl do_rela
1:

    ldr r1, [sp, (AUX_CNT+DT_RELRSZ)*4]
    cmp r1, 0
    beq 1f
    ldr r0, [sp, (AUX_CNT+DT_RELR)*4]
    add r0, r6, r0
    bl do_relr
1:
    add sp, (AUX_CNT + DYN_CNT)*4
    mov r0, r4
    mov r1, r5
    mov r2, r6
    mov fp, 0
    bl _start_c

decode_vec:
    ldr r3, [r0]
    cmp r3, 0
    beq 3f
1:  cmp r2, r3
    bls 2f
    ldr ip, [r0, 4]
    str ip, [r1, r3, lsl 2]
2:
    ldr r3, [r0, 8]!
    cmp r3, 0
    bne 1b
3:  mov pc, lr

find_base:
    @ assume phdr list not empty
    ldr r3, [r0]
    cmp r3, PT_DYNAMIC
    beq 1f
    add r0, r0, r2
    subs r1, r1, 1
    bne find_base
    udf 0
1:
    ldr r3, [r0, 8]
    sub r0, r5, r3
    mov pc, lr

do_rel:
    ldr r3, [r0, 4]
    and r3, 0xff
    cmp r3, R_ARM_RELATIVE
    bne 1f
    ldr r3, [r0]
    ldr r2, [r6, r3]
    add r2, r2, r6
    str r2, [r6, r3]
1:
    add r3, 8
    subs r1, 8
    bne do_rel
    mov pc, lr

do_rela:
    ldr r3, [r0, 4]
    and r3, 0xff
    cmp r3, R_ARM_RELATIVE
    bne 1f
    ldr r3, [r0]
    ldr r2, [r0, 8]
    add r2, r2, r6
    str r2, [r6, r3]
1:
    add r3, 12
    subs r1, 12
    bne do_rela
    mov pc, lr

do_relr:
    mov r2, 0
1:
    ldr r3, [r0], 4
    tst r3, 1
    bne 2f
    @ base address
    add r2, r3, r6
    ldr r3, [r2]
    add r3, r3, r6
    str r3, [r2], 4
    b 3f
2:
    @ bit field
    lsr r3, 1
    mov ip, 31
10:
    tst r3, 1
    beq 11f
    ldr r7, [r2]
    add r7, r7, r6
    str r7, [r2]
11:
    add r2, 4
    lsr r3, 1
    subs r7, 1
    bne 10b
3:
    subs r1, 4
    bne 1b
    mov pc, lr
