.global __tlsdesc_static
.hidden __tlsdesc_static
.type __tlsdesc_static, @function
__tlsdesc_static:
    movq 8(%rax), %rax
    retq
.size __tlsdesc_static, . - __tlsdesc_static

.global __tlsdesc_dynamic
.hidden __tlsdesc_dynamic
.type __tlsdesc_dynamic, @function
__tlsdesc_dynamic:
    pushq %rdx
    pushq %rcx
    movq %fs:32, %rcx                   # rcx = self->dtv
    movq 8(%rax), %rdx                  # rdx = &{tlsmod, tlsoff}
    movq (%rdx), %rax                   # rax = tlsmod
    movq (%rcx, %rax, 8), %rax          # rax = self->dtv[tlsmod]
    subq %fs:0, %rax                    # rax = self->dtv[tlsmod] - self
    addq 8(%rdx), %rax                  # rax = self->dtv[tlsmod] - self + tlsoff
    popq %rcx
    popq %rdx
    retq
.size __tlsdesc_dynamic, . - __tlsdesc_static
