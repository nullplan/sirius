.global sigsetjmp
.type sigsetjmp, @function
sigsetjmp:
    movl 8(%esp), %eax
    testl %eax, %eax
    jz 99f
    movl 4(%esp), %edx
    movl %ebx, 32(%edx)
    movl %edi, 36(%edx)
    movl %edx, %ebx
    movl (%esp), %edi
.hidden __setjmp
    call __setjmp
    movl %ebx, %edx
    movl 32(%edx), %ebx
    movl %edi, (%esp)
    movl 36(%edx), %edi
    movl %edx, 4(%esp)
    movl %eax, 8(%esp)
.hidden __sigsetjmp_tail
    jmp __sigsetjmp_tail

99:
    jmp __setjmp
