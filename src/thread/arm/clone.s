.syntax unified
.global __clone
.hidden __clone
.type __clone, %function
__clone:
@ int __clone(int (*fn)(void *), void *stack, int flags, void *arg, int *ptid, uintptr_t tls, int *ctid);
@                 r0                 r1          r2          r3        [sp]       [sp, 4]      [sp, 8]
@ sys_clone(flags, stack, ptid, tls, ctid)
@            r0     r1      r2   r3   r4
    push {r4, r7}
    cmp r1, 0
    andne r1, -8
    addne r1, -8
    strne r0, [r1]
    strne r3, [r1, 4]
    mov r0, r2
    ldr r2, [sp, 8]
    ldr r3, [sp, 12]
    ldr r4, [sp, 16]
    mov r7, 120
    svc 0
    cmp r1, 0
    beq 1f
    cmp r0, 0
    beq 2f
1:
    pop {r4, r7}
    bx lr
.size __clone, . - __clone

2:
    ldr r0, [sp, 4]
    ldr r1, [sp]
    add sp, 8
    bx r1
    mov r1, r0
    mov r7, 1
1:
    mov r0, r1
    svc 0
    b 1b
