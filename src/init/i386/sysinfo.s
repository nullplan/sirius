.section ".text.oldsyscall","ax",@progbits
1: int $0x80
    ret

.section ".text.__get_sysinfo","ax",@progbits
.global __get_sysinfo
.type __get_sysinfo, @function
.hidden __get_sysinfo
__get_sysinfo:
    movl 4(%esp), %eax
    movl 32*4(%eax), %eax
    testl %eax, %eax
    jnz 2f
    call 3f
3:  popl %eax
    addl $1b-3b, %eax
2:  ret
.size __get_sysinfo, . - __get_sysinfo
