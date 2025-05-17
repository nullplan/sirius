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
    subq $(AUX_NUM+DYN_NUM)*8, %rsp
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

    addq $(AUX_NUM+DYN_NUM)*8, %rsp
    movq %r15, %rdi
    movq %r14, %rsi
    movq %r13, %rdx
    call _start_c
    ud2

decode_vec:
    movq (%rsi), %rax
    testq %rax, %rax
    jz 3f
1:
    cmpq %rdx, %rax
    jae 2f
    movq 8(%rsi), %rcx
    movq %rcx, (%rdi, %rax, 8)
2:
    addq $16, %rsi
    movq (%rsi), %rax
    testq %rax, %rax
    jnz 1b
3:
    retq

find_base:
    movl (%rdi), %eax
    cmpl $PT_DYNAMIC, %eax
    jz 1f
    addq %rdx, %rdi
    decq %rsi
    jnz find_base
    ud2

1:
    movq %r14, %rax
    subq 16(%rdi), %rax
    retq

do_rel:
    testq %rsi, %rsi
    jz 3f
1:
    movl 8(%rdi), %eax
    cmpl $R_X86_64_RELATIVE, %eax
    jnz 2f
    movq (%rdi), %rax
    addq %r13, (%r13, %rax)
2:
    addq $16, %rdi
    subq $16, %rsi
    jnz 1b
3:
    retq

do_rela:
    testq %rsi, %rsi
    jz 3f
1:
    movl 8(%rdi), %eax
    cmpl $R_X86_64_RELATIVE, %eax
    jnz 2f
    movq (%rdi), %rax
    movq 16(%rdi), %r8
    addq %r13, %r8
    movq %r8, (%r13, %rax)
2:
    addq $24, %rdi
    subq $24, %rsi
    jnz 1b
3:
    retq

do_relr:
    xorl %eax, %eax
    testq %rsi, %rsi
    jz 3f
1:
    movq (%rdi), %r8
    testq $1, %r8
    jnz 10f

    leaq 8(%r13, %r8), %rax
    addq %r13, -8(%rax)
    jmp 11f
10:
    shrq %r8
    movl $63, %ecx
20:
    shrq %r8
    jnc 21f
    addq %r13, (%rax)
21:
    addq $8, %rax
    decb %cl
    jnz 20b

11:
    addq $8, %rdi
    subq $8, %rsi
    jnz 1b
3:
    retq
