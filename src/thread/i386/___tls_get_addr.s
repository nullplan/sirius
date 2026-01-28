.global ___tls_get_addr
.type ___tls_get_addr, @function
___tls_get_addr:
    # return __pthread_self()->dtv[eax[0]] + eax[1]
    # return *(*(%gs:16) + eax[0]) + eax[1]
    movl %gs:16, %edx
    movl (%eax), %ecx
    movl 4(%eax), %eax
    addl (%edx, %ecx, 4), %eax
    retl
.size ___tls_get_addr, . - ___tls_get_addr
