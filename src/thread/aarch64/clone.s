.global __clone
.hidden __clone
.type __clone, @function
__clone:
# int __clone(int (*fn)(void *), void *stack, int flags, void *arg, int *ptid, uintptr_t tls, int *ctid);
#                 x0                 x1          w2          x3        x4          x5           x6
# sys_clone(flags, stack, ptid, tls, ctid)
#            x0     x1      x2   x3   x4
    cbz x1, 1f
    and x1, x1, -16
    add x1, x1, -16
    str x0, [x1]
    str x3, [x1, 8]
1:
    mov x8, 220
    mov x0, x2
    mov x2, x4
    mov x3, x5
    mov x4, x6
    svc 0
    cbnz x0, 1f
    cbnz x1, 2f
1:  ret
.size __clone, . - __clone

2:
    mov fp, 0
    ldr x0, [sp, 8]
    ldr x1, [sp]
    blr x1
1:
    mov x8, 93
    svc 0
    b 1b
