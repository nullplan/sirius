.global __tlsdesc_static
.hidden __tlsdesc_static
.type __tlsdesc_static, @function
__tlsdesc_static:
    movl 4(%eax), %eax
    retl
.size __tlsdesc_static, . - __tlsdesc_static

.global __tlsdesc_dynamic
.hidden __tlsdesc_dynamic
.type __tlsdesc_dynamic, @function
__tlsdesc_dynamic:
    pushl %edx
    pushl %ecx
    movl 4(%eax), %eax
    calll ___tls_get_addr@PLT
    subl %gs:0, %eax
    popl %ecx
    popl %edx
    retl
.size __tlsdesc_dynamic, . - __tlsdesc_static
