.syntax unified
.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type __syscall_cp_asm, %function
__syscall_cp_asm:
# long __syscall_cp_asm(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, int *);
#                         r0             r1             r2              r3            [sp]           [sp, 4]        [sp, 8]       [sp, 12]
    push {r4-r7}
    ldr r6, [sp, 28]
.global __cp_begin
.hidden __cp_begin
__cp_begin:
    ldr r6, [r6]
    cmp r6, 0
    bne __cp_cancel
    ldr r4, [sp, 16]
    ldr r5, [sp, 20]
    ldr r7, [sp, 24]
    svc 0
.global __cp_end
.hidden __cp_end
__cp_end:
    pop {r4-r7}
    bx lr
.size __syscall_cp_asm, . - __syscall_cp_asm

.global __cp_cancel
.hidden __cp_cancel
__cp_cancel:
.hidden __cancel
    pop {r4-r7}
    b __cancel
