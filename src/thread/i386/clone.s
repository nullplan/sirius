.global __clone
.type __clone, @function
.hidden __clone
__clone:
    pushl %ebp
    movl %esp, %ebp
# int __clone(int (*fn)(void *), void *stack, int flags, void *arg, int *ptid, uintptr_t tls, int * ctid);
#                  8(%ebp)           12(%ebp)    16(%ebp)  20(%ebp)    24(%ebp)     28(%ebp)    32(%ebp)
# SYS_clone(flags, stack, ptid, tls, ctid)
# but the tls must be a user desc pointer
    subl $40, %esp
    movl %esi, 36(%esp)
    movl %edi, 32(%esp)
    movl %ebx, 28(%esp)
    xorl %eax, %eax
    movw %gs, %ax
    shrl $3, %eax
    movl %eax, (%esp)
    movl 28(%ebp), %eax
    movl %eax, 4(%esp)
    movl $0xfffff, 8(%esp)
    movl $0x51, 12(%esp)
    movl 12(%ebp), %ecx
    testl %ecx, %ecx
    jz 1f
    andl $-16, %ecx
    addl $-16, %ecx
    movl 20(%ebp), %edx
    movl 8(%ebp), %eax
    movl %edx, (%ecx)
    movl %eax, 4(%ecx)
1:
    movl $120, %eax
    movl 16(%ebp), %ebx
    movl 24(%ebp), %edx
    movl %esp, %esi
    movl 32(%ebp), %edi
    int $0x80
    testl %eax, %eax
    jnz 1f
    testl %ecx, %ecx
    jnz 2f
1:
    movl 28(%esp), %ebx
    movl 32(%esp), %edi
    movl 36(%esp), %esi
    movl %ebp, %esp
    popl %ebp
    retl
.size __clone, . - __clone

2:
    xorl %ebp, %ebp
    calll *4(%esp)
    movl %eax, %ebx
1:
    movl $1, %eax
    int $0x80
    jmp 1b
