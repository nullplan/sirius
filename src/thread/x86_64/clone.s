.global __clone
.hidden __clone
.type __clone, @function
__clone:
# int __clone(int (*fn)(void *), void *stack, int flags, void *arg, int *ptid, uintptr_t tls, int *ctid);
#                  %rdi              %rsi        %rdx      %rcx         %r8       %r9          8(%rsp)
# sys_clone(flags, stack, ptid, ctid, tls)
#            %rdi  %rsi   %rdx   %r10  %r8
    testq %rsi, %rsi
    jz 1f
    andq $-16, %rsi
    addq $-16, %rsi
    movq %rdi, (%rsi)
    movq %rcx, 8(%rsi)
1:
    movl $56, %eax
    movq %rdx, %rdi
    movq %r8, %rdx
    movq 8(%rsp), %r10
    movq %r9, %r8
    syscall
    testl %eax, %eax
    jnz 1f
    testq %rsi, %rsi
    jnz 2f
1:
    retq
.size __clone, . - __clone

2:
    movq 8(%rsp), %rdi
    callq *(%rsp)
1:
    movl $60, %eax
    syscall
    jmp 1b
