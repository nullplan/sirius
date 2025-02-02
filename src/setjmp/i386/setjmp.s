.global setjmp
.type setjmp, @function
.global __setjmp
.hidden __setjmp
setjmp:
__setjmp:
    movl 4(%esp), %edx
    leal 4(%esp), %ecx
    movl (%esp), %eax
    movl %ebx, (%edx)
    movl %esi, 4(%edx)
    movl %edi, 8(%edx)
    movl %ebp, 12(%edx)
    movl %ecx, 16(%edx)
    movl %eax, 20(%edx)
    xorl %eax, %eax
    retl
.size setjmp, . - setjmp
