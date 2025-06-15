1: int $0x80
    ret

.data
.global __sysinfo
.hidden __sysinfo
.type __sysinfo, @object
__sysinfo: .long 1b
.size __sysinfo, . - __sysinfo

.text
.global __init_sysinfo
.type __init_sysinfo, @function
.hidden __init_sysinfo
__init_sysinfo:
    movl 4(%esp), %eax
    movl 32*4(%eax), %eax
    testl %eax, %eax
    jz 2f
    call 1f
1:
    popl %ecx
    movl %eax, __sysinfo-1b(%ecx)
2:  ret
.size __init_sysinfo, . - __init_sysinfo
