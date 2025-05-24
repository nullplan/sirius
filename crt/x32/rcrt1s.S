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
    movl %esp, %r15d
    leal _DYNAMIC(%rip), %r14d
    andl $-16, %esp
    subl $(AUX_NUM+DYN_NUM)*4, %esp
    movl %esp, %edi
    xorl %eax, %eax
    movl $AUX_NUM+DYN_NUM, %ecx
    rep
    stosl
    movl (%r15d), %eax
    leal 8(%r15d, %eax, 4), %esi
1:
    lodsl
    testl %eax, %eax
    jnz 1b
    movl %esp, %edi
    movl $AUX_NUM, %edx
    call decode_vec
    movl %r14d, %esi
    leal AUX_NUM*4(%esp), %edi
    movl $DYN_NUM, %edx
    call decode_vec

    movl AT_BASE*4(%esp), %eax
    testl %eax, %eax
    jnz 1f
    movl AT_PHDR*4(%esp), %edi
    movl AT_PHNUM*4(%esp), %esi
    movl AT_PHENT*4(%esp), %edx
    call find_base
1:
    movl %eax, %r13d

    movl (AUX_NUM+DT_REL)*4(%esp), %edi
    movl (AUX_NUM+DT_RELSZ)*4(%esp), %esi
    addl %r13d, %edi
    call do_rel

    movl (AUX_NUM+DT_RELA)*4(%esp), %edi
    movl (AUX_NUM+DT_RELASZ)*4(%esp), %esi
    addl %r13d, %edi
    call do_rela

    movl (AUX_NUM+DT_RELR)*4(%esp), %edi
    movl (AUX_NUM+DT_RELRSZ)*4(%esp), %esi
    addl %r13d, %edi
    call do_relr

    addl $(AUX_NUM+DYN_NUM)*4, %esp
    movl %r15d, %edi
    movl %r14d, %esi
    movl %r13d, %edx
.hidden _start_c
    call _start_c
    ud2
.size _start, . - _start

decode_vec:
    movl (%esi), %eax
    testl %eax, %eax
    jz 3f
1:
    cmpl %edx, %eax
    jae 2f
    movl 4(%esi), %ecx
    movl %ecx, (%edi, %eax, 4)
2:
    addl $8, %esi
    movl (%esi), %eax
    testl %eax, %eax
    jnz 1b
3:
    ret

find_base:
1:
    movl (%edi), %eax
    cmpl $PT_DYNAMIC, %eax
    jz 2f
    addl %edx, %edi
    decl %esi
    jnz 1b
    ud2
2:
    movl %r14d, %eax
    subl 8(%edi), %eax
    ret
    
do_rel:
    testl %esi, %esi
    jz 3f
1:
    movl 4(%edx), %eax
    andl $0xff, %eax
    cmpl $R_X86_64_RELATIVE, %eax
    jnz 2f
    movl (%edx), %eax
    addl $r13d, (%r13, %rax)
2:
    addl $8, %edx
    subl $8, %esi
    jnz 1b
3:
    ret

do_rela:
    testl %esi, %esi
    jz 3f
1:
    movl 4(%edx), %eax
    andl $0xff, %eax
    cmpl $R_X86_64_RELATIVE, %eax
    jnz 2f
    movl (%edx), %eax
    movl 8(%edx), %ecx
    addl %r13d, %ecx
    movl %ecx, (%r13, %rax)
2:
    addl $12, %edx
    subl $12, %esi
    jnz 1b
3:
    ret

do_relr:
    xorl %edx, edx
    testl %esi, %esi
    jnz 4f
1:
    movl (%edi), %eax
    testl $1, %eax
    jnz 2f
    /* base address */
    movl %eax, %edx
    addl %r13d, (%edx)
    addl $4, %edx
    jmp 3f
2:
    shrl %eax
    movl $31, %ecx
10:
    shrl %eax
    jnc 11f
    addl %r13d, (%edx)
11:
    addl $4, %edx
    decl %ecx
    jnz 10b
3:
    addl $4, %edi
    decl %esi
    jnz 1b
4:
    ret
