.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type __syscall_cp_asm, @function
__syscall_cp_asm:
# long __syscall_cp_asm(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, int *);
#                         r3              r4             r5             r6             r7              r8             r9          r10
.global __cp_begin
.hidden __cp_begin
__cp_begin:
    lwz %r10, 0(%r10)
    cmplwi %r10, 0
    bne __cp_cancel
    mr %r0, %r9
    sc
.global __cp_end
.hidden __cp_end
__cp_end:
    bnslr
    neg %r3, %r3
    blr
.size __syscall_cp_asm, . - __syscall_cp_asm

.global __cp_cancel
.hidden __cp_cancel
__cp_cancel:
.hidden __cancel
    b __cancel
