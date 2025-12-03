.syntax unified
.global __aeabi_read_tp
.type __aeabi_read_tp, %function
__aeabi_read_tp:
    ldr r0, 1f
    add r0, pc, r0
    ldr r0, [r0]
2:  bx r0
.balign 4
.hidden __a_gettp_ptr
1: .word __a_gettp_ptr - 2b
.size __aeabi_read_tp, . - __aeabi_read_tp
