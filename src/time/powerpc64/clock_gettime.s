.section ".rodata.str","aMS",@progbits, 1
.Lcgt_name: .asciz "__kernel_clock_gettime"
.Lcgt_ver: .asciz "LINUX_2.6.15"

.section ".data.impl","aw",@progbits
.balign 8
.Limpl: .quad .Lcgt_dispatch

.section ".text.cgt_syscall","ax",@progbits
.Lcgt_syscall:
    li %r0, 246
    sc
    blr

.section ".text.cgt_dispatch","ax",@progbits
# Always called internally, so r2==.TOC.
.Lcgt_dispatch:
    mflr %r0
    std %r0, 16(%r1)
    stdu %r1, -64(%r1)
    std %r3, 32(%r1)
    std %r4, 40(%r1)

    addis %r3, %r2, .Lcgt_name@toc@ha
    addi %r3, %r3, .Lcgt_name@toc@l
    addis %r4, %r2, .Lcgt_ver@toc@ha
    addi %r4, %r4, .Lcgt_ver@toc@l
    bl __vdsosym
    cmpldi %r3, 0
    bne 1f
    addis %r3, %r2, .Lcgt_syscall@toc@ha
    addi %r3, %r3, .Lcgt_syscall@toc@l
1:
    addis %r4, %r2, .Limpl@toc@ha
    std %r3, .Limpl@toc@l(%r4)
    sync
    mr %r12, %r3
    mtctr %r3
    ld %r0, 80(%r1)
    ld %r3, 32(%r1)
    ld %r4, 40(%r1)
    addi %r1, %r1, 64
    mtlr %r0
    bctr

.section ".text.clock_gettime","ax",@progbits
.global __clock_gettime
.type __clock_gettime,@function
.hidden __clock_gettime
__clock_gettime:
    addis %r2, %r12, .TOC. - __clock_gettime@ha
    addi %r2, %r2, .TOC. - __clock_gettime@l
    .localentry __clock_gettime, . - __clock_gettime
    mflr %r0
    std %r0, 16(%r1)
    stdu %r1, -32(%r1)
    addis %r11, %r2, .Limpl@toc@ha
    ld %r12, .Limpl@toc@l(%r11)
    mtctr %r12
    bctrl
    bns 1f
    stw %r3, -0x7020(%r13)
    li %r3, -1
1:
    ld %r0, 48(%r1)
    addi %r1, %r1, 32
    mtlr %r0
    blr
.size __clock_gettime, . - __clock_gettime

.global clock_gettime
.weak clock_gettime
.set clock_gettime, __clock_gettime
