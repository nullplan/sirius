# the argument is in x0, and I return the offset relative to tpidr_el0
.global __tlsdesc_static
.hidden __tlsdesc_static
.type __tlsdesc_static, @function
__tlsdesc_static:
    ldr x0, [x0, 8]
    ret
.size __tlsdesc_static, . - __tlsdesc_static

.global __tlsdesc_dynamic
.hidden __tlsdesc_dynamic
.type __tlsdesc_dynamic, @function
__tlsdesc_dynamic:
    stp x1, x2, [sp, -16]!
    ldr x0, [x0, 8]     // x0 = &(size_t[]){tlsmod, tlsoff}
    mrs x1, tpidr_el0
    ldp x0, x2, [x0]    // x0 = tlsmod, x2 = tlsoff
    sub x2, x2, x1      // x2 = tlsoff - tp
    ldr x1, [x1, -8]    // x1 = dtv
    ldr x1, [x1, x0, lsl 3] // x1 = dtv[tlsmod]
    add x0, x1, x2          // x0 = dtv[tlsmod] + tlsoff - tp
    ldp x1, x2, [sp], 16
    ret
.size __tlsdesc_dynamic, . - __tlsdesc_static
