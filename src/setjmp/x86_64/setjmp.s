.global setjmp
.type setjmp, @function
.global __setjmp
.hidden __setjmp
setjmp:
__setjmp:
    movq (%rsp), %rdx
    leaq 8(%rsp), %rcx
    movq %rbx, (%rdi)
    movq %rbp, 8(%rdi)
    movq %rcx, 16(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 32(%rdi)
    movq %r14, 40(%rdi)
    movq %r15, 48(%rdi)
    movq %rdx, 56(%rdi)
    xorl %eax, %eax
    retq
.size setjmp, . - setjmp
