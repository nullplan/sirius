1: int $0x80
    ret

.global __get_sysinfo
.type __get_sysinfo, @function
.hidden __get_sysinfo
__get_sysinfo:
    movl 4(%esp), %eax
    movl 32*4(%eax), %eax
    testl %eax, %eax
    jnz 2f
    call 3f
4:  addl $1b-4b, %eax
2:  ret
.size __get_sysinfo, . - __get_sysinfo

3:
    movl (%esp), %eax
    retl
