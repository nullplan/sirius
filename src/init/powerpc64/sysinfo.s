.Lsc:
    sc
    bnslr
    neg %r3, %r3
    blr

.Lscv:
    mflr %r0
    stdu %r0, -32(%r1)
    .long (17 << 26) + 1
    ld %r0, 0(%r1)
    addi %r1, %r1, 32
    mtlr %r0
    blr

.data
.align 8
.global __sysinfo
.type __sysinfo, @object
.hidden __sysinfo
__sysinfo:
    .quad .Lsc
.size __sysinfo, . - __sysinfo

.text
.global __init_sysinfo
.type __init_sysinfo, @function
.hidden __init_sysinfo
__init_sysinfo:
    addis %r2, %r12, .TOC. - __init_sysinfo@ha
    addi %r2, %r2, .TOC. - __init_sysinfo@l
    .localentry __init_sysinfo, . - __init_sysinfo
    ld %r3, 26*8(%r3)
    andis. %r3, %r3, 0x0010
    beqlr
    addis %r3, %r2, .Lscv@toc@ha
    addi %r3, %r3, .Lscv@toc@l
    addis %r4, %r2, __sysinfo@toc@ha
    std %r3, __sysinfo@toc@l(%r4)
    blr
.size __init_sysinfo, . - __init_sysinfo
