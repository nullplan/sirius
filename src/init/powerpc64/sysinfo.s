.Lsc:
    sc
    bnslr
    neg %r3, %r3
    blr

.Lscv:
    mflr %r12
    stdu %r12, -32(%r1)
    .long 0x44000001
    ld %r12, 0(%r1)
    addi %r1, %r1, 32
    mtlr %r12
    blr

.global __get_sysinfo
.type __get_sysinfo, @function
.hidden __get_sysinfo
__get_sysinfo:
    addis %r2, %r12, .TOC. - __get_sysinfo@ha
    addi %r2, %r2, .TOC. - __get_sysinfo@l
    .localentry __get_sysinfo, . - __get_sysinfo
    ld %r3, 26*8(%r3)
    andis. %r3, %r3, 0x0010
    addis %r3, %r2, .Lsc@toc@ha
    addi %r3, %r3, .Lsc@toc@l
    beqlr
    addi %r3, %r3, .Lscv - .Lsc
    blr
.size __get_sysinfo, . - __get_sysinfo
