    nop
.global __sigrestore
.hidden __sigrestore
.global __rt_sigrestore
.hidden __rt_sigrestore
__sigrestore:
__rt_sigrestore:
    movl $0x40000201, %eax
    syscall

