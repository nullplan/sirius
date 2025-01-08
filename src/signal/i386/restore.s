    nop
.global __sigrestore
.hidden __sigrestore
__sigrestore:
    popl %eax
    movl $119, %eax
    int $0x80

.global __rt_sigrestore
.hidden __rt_sigrestore
__rt_sigrestore:
    movl $173, %eax
    int $0x80
