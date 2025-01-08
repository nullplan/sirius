.global __sigrestore
.hidden __sigrestore
    nop
__sigrestore:
    li r0, 119
    sc

.global __rt_sigrestore
.hidden __rt_sigrestore
    nop
__rt_sigrestore:
    li r0, 172
    sc
