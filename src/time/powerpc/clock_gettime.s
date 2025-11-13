# All the options with C and inline ASM are worse!
.section ".rodata.str","aMS",@progbits, 1
.Lcgt_name: .asciz "__kernel_clock_gettime64"
.Lcgt_ver: .asciz "LINUX_2.6.15"

.section ".data.impl","aw",@progbits
.balign 4
.Limpl: .long .Lcgt_dispatch

.section ".text.cgt_syscall","ax",@progbits
.Lcgt_syscall:
    li %r0, 403
    sc
    blr

.section ".text.cgt_dispatch","ax",@progbits
.Lcgt_dispatch:
    mflr %r0
    stw %r0, 4(%r1)
    stwu %r1, -32(%r1)
    stw %r3, 8(%r1)
    stw %r4, 12(%r1)
    stw %r31, 16(%r1)

    bcl 20,31,1f
1:
    mflr %r31
    addis %r3, %r31, .Lcgt_name-1b@ha
    addi %r3, %r3, .Lcgt_name-1b@l
    addis %r4, %r31, .Lcgt_ver-1b@ha
    addi %r4, %r4, .Lcgt_ver-1b@l
.hidden __vdsosym
    bl __vdsosym
    cmplwi %r3, 0
    bne 2f
    addis %r3, %r31, .Lcgt_syscall-1b@ha
    addi %r3, %r3, .Lcgt_syscall-1b@l
2:
    addis %r4, %r31, .Limpl-1b@ha
    stw %r3, .Limpl-1b@l(%r4)
    sync
    mtctr %r3
    lwz %r0, 36(%r1)
    lwz %r3, 8(%r1)
    lwz %r4, 12(%r1)
    lwz %r31, 16(%r1)
    addi %r1, %r1, 32
    mtlr %r0
    bctr

.section ".text.clock_gettime","ax",@progbits
.global __clock_gettime
.type __clock_gettime,@function
.hidden __clock_gettime
.global clock_gettime
.type clock_gettime,@function
.weak clock_gettime
__clock_gettime:
clock_gettime:
    mflr %r0
    stw %r0, 4(%r1)
    stwu %r1, -16(%r1)
    bcl 20,31,1f
1:
    mflr %r12
    addis %r12, %r12, .Limpl-1b@ha
    lwz %r12, .Limpl-1b@l(%r12)
    mtctr %r12
    bctrl
    bns 1f
    stw %r3, -0x7010(%r2)
    li %r3, -1
1:
    lwz %r0, 20(%r1)
    addi %r1, %r1, 16
    mtlr %r0
    blr
.size __clock_gettime, . - __clock_gettime
.size clock_gettime, . - clock_gettime
