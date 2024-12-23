1: int $0x80
    ret

.data
.global __sysinfo
.hidden __sysinfo
.type __sysinfo, @object
__sysinfo: .long 1b
.size __sysinfo, . - __sysinfo
