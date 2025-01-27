/* relevant parts of elf.h */
#define PT_DYNAMIC  2
#define R_X86_64_RELATIVE 8
#define AT_PHDR   3
#define AT_PHENT  4
#define AT_PHNUM  5
#define AT_BASE   7
#define DT_RELA     7
#define DT_RELASZ   8
#define DT_REL  17
#define DT_RELSZ    18
#define DT_RELRSZ	35
#define DT_RELR		36

#define AUX_NUM (AT_BASE+1)
#define DYN_NUM (DT_RELR+1)

.global _start
.type _start, @function
_start:
    xorl %ebp, %ebp
    movq %rsp, %r15
    andq $-16, %rsp
    subq (AUX_NUM+DYN_NUM)*8, %rsp
    movq %rsp, %rdi
    xorl %eax, %eax
    movl $(AUX_NUM+DYN_NUM), %ecx
    rep
    stosq
    leaq _DYNAMIC(%rip), %r14
    movq (%r15), %rax
    leaq 16(%r15, %rax, 8), %rsi
1:
    lodsq
    testq %rax, %rax
    jnz 1b
    movq %rsp, %rdi
    movl $AUX_NUM, %edx
    call decode_vec
    leaq AUX_NUM*8(%rsp), %rdi
    movq %r14, %rsi
    movl $DYN_NUM, %edx
    call decode_vec

    movq AT_BASE*8(%rsp), %rax
    testq %rax, %rax
    jnz 1f
    movq AT_PHDR*8(%rsp), %rdi
    movq AT_PHNUM*8(%rsp), %rsi
    movq AT_PHENT*8(%rsp), %rdx
    call find_base
1:
    movq %rax, %r13

    movq (AUX_NUM+DT_REL)*8(%rsp), %rdi
    movq (AUX_NUM+DT_RELSZ)*8(%rsp), %rsi
    addq %r13, %rdi
    call do_rel

    movq (AUX_NUM+DT_RELA)*8(%rsp), %rdi
    movq (AUX_NUM+DT_RELASZ)*8(%rsp), %rsi
    addq %r13, %rdi
    call do_rela

    movq (AUX_NUM+DT_RELR)*8(%rsp), %rdi
    movq (AUX_NUM+DT_RELRSZ)*8(%rsp), %rsi
    addq %r13, %rdi
    call do_relr

    addq (AUX_NUM+DYN_NUM)*8, %rsp
    movq %r15, %rdi
    call _start_c
    ud2
