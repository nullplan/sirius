.global __clone
.hidden __clone
.type __clone, @function
__clone:
# int __clone(int (*fn)(void *), void *stack, int flags, void *arg, int *ptid, uintptr_t tls, int *ctid);
#                 %edi                %esi        %edx      %ecx        %r8d     %r9d          8(%rsp)
# sys_clone(flags, stack, ptid, ctid, tls)
#           %edi   %esi   %edx  %r10d %r8d
    testl %esi, %esi
    jz 1f
    andl $-16, %esi
    addl $-16, %esi
    movl %edi, (%esi)
    movl %ecx, 4(%esi)
1:
    movl $0x40000038, %eax
    movl %edx, %edi
    movl %r8d, %edx
    movl 8(%esp), %r10d
    movl %r9d, %r8d
    syscall
    testl %eax, %eax
    jnz 1f
    testl %esi, %esi
    jnz 2f
1:
    retl
.size __clone, . - __clone

2:
    movl 4(%esp), %edi
    movl (%esp), %eax
    callq *%rax
    movl %eax, %edi
1:
    movl $0x4000003c, %eax
    syscall
    jmp 1b
