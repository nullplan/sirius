.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type __syscall_cp_asm, @function
__syscall_cp_asm:
# long __syscall_cp_asm(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, int *);
#                          rdi             rsi              rdx          rcx           r8             r9            8(%rsp)      16(%rsp)
    movq 8(%rsp), %rax
    movzlq 16(%rsp), %r10
.global __cp_begin
.hidden __cp_begin
__cp_begin:
    movl (%r10), %r10d
    testl %r10d, %r10d
    jnz __cp_cancel
    movq %rcx, %r10
    syscall
.global __cp_end
.hidden __cp_end
__cp_end:
    ret
.size __syscall_cp_asm, . - __syscall_cp_asm

.global __cp_cancel
.hidden __cp_cancel
__cp_cancel:
.hidden __cancel
    jmp __cancel
