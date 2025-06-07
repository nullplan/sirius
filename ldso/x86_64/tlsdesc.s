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
    pushq %rcx
    pushq %rdx
    movq 8(%rax), %rax
    movq (%rax), %rdx
    movq %fs:32, %rcx
    movq (%rcx, %rdx, 8), %rdx
    movq 8(%rax), %rax
    addq %rdx, %rax
    subq %fs:0, %rax
    popq %rdx
    popq %rcx
    retq
.size __tlsdesc_dynamic, . - __tlsdesc_static
