/* relevant parts of elf.h */
#define PT_DYNAMIC  2
#define R_386_RELATIVE 8
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
    movl %esp, %ebp
    andl $-16, %esp
    subl $(AUX_NUM+DYN_NUM)*4, %esp
    movl %esp, %edi
    movl $AUX_NUM+DYN_NUM, %ecx
    xorl %eax, %eax
    rep
    stosl
    movl (%ebp), %esi
    leal 8(%ebp, %esi, 4), %esi
1:
    lodsl
    testl %eax, %eax
    jnz 1b
    movl %esp, %eax
    pushl $AUX_NUM
    pushl %esi
    pushl %eax
    call decode_vec
    addl $12, %esp
    call 1f
1:
    popl %edi
    addl $_DYNAMIC-1b, %edi
    leal AUX_NUM*4(%esp),%eax
    pushl $DYN_NUM
    pushl %edi
    pushl %eax
    call decode_vec
    addl $12, %esp

    movl AT_BASE*4(%esp), %eax
    testl %eax, %eax
    jnz 1f
    pushl AT_PHENT*4(%esp)
    pushl AT_PHNUM*4+4(%esp)
    pushl AT_PHDR*4+8(%esp)
    call find_base
    addl $12, %esp
1:
    movl %eax, %ebx

    movl (AUX_NUM+DT_REL)*4(%esp), %eax
    addl %ebx, %eax
    pushl %eax
    pushl (AUX_NUM+DT_RELSZ)*4+4(%esp)
    call do_rel
    addl $8, %esp

    movl (AUX_NUM+DT_RELA)*4(%esp), %eax
    addl %ebx, %eax
    pushl %eax
    pushl (AUX_NUM+DT_RELASZ)*4+4(%esp)
    call do_rela
    addl $8, %esp

    movl (AUX_NUM+DT_RELR)*4(%esp), %eax
    addl %ebx, %eax
    pushl %eax
    pushl (AUX_NUM+DT_RELRSZ)*4+4(%esp)
    call do_relr

    addl $8+(AUX_NUM+DYN_NUM)*4-16, %esp
    movl %ebp, (%esp)
    movl %edi, 4(%esp)
    movl %ebx, 8(%esp)
.hidden _start_c
    call _start_c
    ud2
.size _start, . - _start

decode_vec:
    pushl %esi
    movl 8(%esp), %edx
    movl 12(%esp), %esi

    movl (%esi), %eax
    testl %eax, %eax
    jz 3f
1:
    cmpl 16(%esp), %eax
    jae 2f
    movl 4(%esi), %ecx
    movl %ecx, (%edx, %eax, 4)
2:
    addl $8, %esi
    movl (%esi), %eax
    testl %eax, %eax
    jnz 1b
3:
    popl %esi
    retl

find_base:
    pushl %esi
    movl 8(%esp), %esi
    movl 12(%esp), %ecx
    movl 16(%esp), %edx
1:
    movl (%esi), %eax
    cmpl $PT_DYNAMIC, %eax
    jz 2f
    addl %edx, %esi
    decl %ecx
    jnz 1b
    ud2
2:
    movl %edi, %eax
    subl 8(%esi), %eax
    popl %esi
    retl

do_rel:
    pushl %esi
    movl 8(%esp), %esi
    movl 12(%esp), %ecx
    testl %ecx, %ecx
    jz 3f
1:
    movl 4(%esi), %eax
    andl $0xff, %eax
    cmpl $R_386_RELATIVE, %eax
    jnz 2f
    movl (%esi), %eax
    addl %ebx, (%ebx, %eax)
2:
    addl $8, %esi
    subl $8, %ecx
    jnz 1b
3:
    popl %esi
    retl

do_rela:
    pushl %esi
    movl 8(%esp), %esi
    movl 12(%esp), %ecx
    testl %ecx, %ecx
    jz 3f
1:
    movl 4(%esi), %eax
    andl $0xff, %eax
    cmpl $R_386_RELATIVE, %eax
    jnz 2f
    movl (%esi), %eax
    movl 8(%esi), %edx
    addl %ebx, %edx
    movl %edx, (%ebx, %eax)
2:
    addl $12, %esi
    subl $12, %ecx
    jnz 1b
3:
    popl %esi
    retl

do_relr:
    pushl %edi
    pushl %esi
    xorl %edi, %edi
    movl 12(%esp), %esi
    movl 16(%esp), %ecx
    testl %ecx, %ecx
    jz 2f
1:
    lodsl
    testl $1, %eax
    jnz 10f
    leal (%ebx, %eax), %edi
    addl %ebx, (%edi)
    addl $4, %edx
    jmp 11f
10:
    shrl %eax
    movl $31, %edx
20:
    testl $1, %eax
    jz 21f
    addl %ebx, (%edi)
21:
    addl $4, %edi
    decl %edx
    jnz 20b
11:
    subl $4, %ecx
    jnz 1b
2:
    popl %esi
    popl %edi
    retl
