.global sigsetjmp
.type sigsetjmp, @function
sigsetjmp:
    testl %esi, %esi
    jz 1f
    movq %r12, 80(%rdi)
    movq %r13, 88(%rdi)
    movq (%rsp), %r12
    movq %rdi, %r13
.hidden __setjmp
    call __setjmp
    movq %r12, (%rsp)
    movq %r13, %rdi
    movl %eax, %esi
.hidden __sigsetjmp_tail
    jmp __sigsetjmp_tail

1:
    jmp __setjmp
