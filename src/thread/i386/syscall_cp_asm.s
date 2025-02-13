.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type __syscall_cp_asm, @function
__syscall_cp_asm:
# long __syscall_cp_asm(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, int *);
#                          4(%esp)      8(%esp)        12(%esp)         16(%esp)       20(%esp)      24(%esp)      28(%esp)       32(%esp)
    pushl %ebp
    pushl %esi
    pushl %edi
    pushl %ebx
    movl 16+32(%esp), %eax
.global __cp_begin
.hidden __cp_begin
__cp_begin:
    movl (%eax), %eax
    testl %eax, %eax
    jnz __cp_cancel
    movl 16+4(%esp), %ebx
    movl 16+8(%esp), %ecx
    movl 16+12(%esp), %edx
    movl 16+16(%esp), %esi
    movl 16+20(%esp), %edi
    movl 16+24(%esp), %ebx
    movl 16+28(%esp), %eax
    int $0x80
.global __cp_end
.hidden __cp_end
__cp_end:
    popl %ebx
    popl %edi
    popl %esi
    popl %ebp
    ret
.size __syscall_cp_asm, . - __syscall_cp_asm

.global __cp_cancel
.hidden __cp_cancel
__cp_cancel:
    popl %ebx
    popl %edi
    popl %esi
    popl %ebp
.hidden __cancel
    jmp __cancel
