.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type __syscall_cp_asm, @function
__syscall_cp_asm:
# long __syscall_cp_asm(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, int *);
#                         x0             x1             x2              x3            x4                x5           x6           x7
.global __cp_begin
.hidden __cp_begin
__cp_begin:
    ldr w8, [x7]
    cbnz w8, __cp_cancel
    mov x8, x6
    svc 0
.global __cp_end
.hidden __cp_end
__cp_end:
    ret
.size __syscall_cp_asm, . - __syscall_cp_asm

.global __cp_cancel
.hidden __cp_cancel
__cp_cancel:
.hidden __cancel
    b __cancel
