.global __clone
.type __clone, @function
.hidden __clone
__clone:
# pseudo-operation:
# - if stack ptr is given, stash function ptr and arg on child stack
# - shuffle args
# call SYS_clone
# if it returns nonzero or child stack was zero, return
# otherwise prepare stack, execute function, call exit with the result

# all volatile registers get shredded by system call, so I need one non-vol reg of space.
    stw %r31, 4(%r1)    # abuse LR slot

# extern hidden int __clone(int (*fn)(void *), void *stack, int flags, void *arg, int *ptid, uintptr_t tls, int *ctid);
#                               r3                  r4          r5          r6        r7        r8             r9
# SYS_clone(flags, void *stack, int *ptid, unsigned long tls, int *ctid)
#            r3       r4           r5          r6               r7
    li %r31, 0
    cmplwi %r4, 0
    beq 1f
    clrrwi %r4, %r4, 4
    stwu %r31, -16(%r4)
    stw %r31, 4(%r4)
    stw %r3, 8(%r4)
    stw %r6, 12(%r4)
    li %r31, 1
1:
    li %r0, 120
    mr %r3, %r5
    mr %r5, %r7
    mr %r6, %r8
    mr %r7, %r9
    sc
    cmpwi %r3, 0
    bne 1f
    cmpwi %r31, 0
    bne 2f
1:
    lwz %r31, 4(%r1)
    blr
.size __clone, . - __clone

2:
    lwz %r3, 12(%r1)
    lwz %r4, 8(%r1)
    mtctr %r4
    bctrl
    mr %r31, %r3
1:
    li %r0, 1
    sc
    mr %r3, %r31
    b 1b
