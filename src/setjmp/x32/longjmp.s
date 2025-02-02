.global longjmp
.type longjmp, @function
longjmp:
    movl $1, %eax
    testl %esi, %esi
    cmovnzl %esi, %eax
    movq (%rdi), %rbx
    movq 8(%rdi), %rbp
    movq 16(%rdi), %rsp
    movq 24(%rdi), %r12
    movq 32(%rdi), %r13
    movq 40(%rdi), %r14
    movq 48(%rdi), %r15
    jmpq *56(%rdi)
.size longjmp, . - longjmp
