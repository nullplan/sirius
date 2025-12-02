.global __sigrestore
.hidden __sigrestore
__sigrestore:
    mov r7, #119
    svc 0

.global __rt_sigrestore
.hidden __rt_sigrestore
__rt_sigrestore:
    mov r7, #173
    svc 0
