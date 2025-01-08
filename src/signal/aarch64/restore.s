    nop
.global __sigrestore
.hidden __sigrestore
.global __rt_sigrestore
.hidden __rt_sigrestore
__sigrestore:
__rt_sigrestore:
  mov x8, 139
  svc 0
