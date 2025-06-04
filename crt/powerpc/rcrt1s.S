/* relevant parts of elf.h */
#define PT_DYNAMIC  2
#define R_PPC_RELATIVE 22
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
    li %r3, AUX_NUM+DYN_NUM
    mr %r31,%r1
    clrrwi %r1,%r1,4
    li %r0, 0
    mtctr %r3
1:
    stwu %r0, -4(%r1)
    bdnz 1b

    bcl 20,31,1f
1:
    mflr %r30
    addis %r30,%r30,_DYNAMIC-1b@ha
    addi %r30,%r30,_DYNAMIC-1b@l
    lwz %r3, 0(%r31)    /* argc */
    slwi %r3, %r3, 2
    add %r3, %r31, %r3
    addi %r3, %r3, 4    /* r3 = just before envp */
1:
    lwzu %r4, 4(%r3)
    cmplwi %r4, 0
    bne 1b
    addi %r4, %r3, 4
    mr %r3, %r1
    li %r5, AUX_NUM
    bl decode_vec
    addi %r3, %r1, AUX_NUM*4
    mr %r4, %r30
    li %r5, DYN_NUM
    bl decode_vec

    lwz %r3, AT_BASE*4(%r1)
    cmplwi %r3, 0
    bne 1f
    lwz %r3, AT_PHDR*4(%r1)
    lwz %r4, AT_PHNUM*4(%r1)
    lwz %r5, AT_PHENT*4(%r1)
    bl find_base
1:
    mr %r29, %r3
    lwz %r3, (AUX_NUM+DT_REL)*4(%r1)
    lwz %r4, (AUX_NUM+DT_RELSZ)*4(%r1)
    add %r3, %r3, %r29
    bl do_rel

    lwz %r3, (AUX_NUM+DT_RELA)*4(%r1)
    lwz %r4, (AUX_NUM+DT_RELASZ)*4(%r1)
    add %r3, %r3, %r29
    bl do_rela

    lwz %r3, (AUX_NUM+DT_RELR)*4(%r1)
    lwz %r4, (AUX_NUM+DT_RELRSZ)*4(%r1)
    add %r3, %r3, %r29
    bl do_relr

    li %r0, 0
    stwu %r0, (AUX_NUM+DYN_NUM)*4-16(%r1)
    mtlr %r0
    mr %r3, %r31
    mr %r4, %r30
    mr %r5, %r29
.hidden _start_c
    b _start_c
.size _start, . - _start

decode_vec:
/* %r3 - dst array
 * %r4 - src vector
 * %r5 - limit
 */
    lwz %r6, 0(%r4)
    cmplwi %r6, 0
    beq- 3f
1:
    cmplw %r6, %r5
    bnl 2f
    lwz %r7, 4(%r4)
    slwi %r6, %r6, 2
    stwx %r7, %r6, %r3
2:
    lwzu %r6, 8(%r4)
    cmplwi %r6, 0
    bne 1b
3:
    blr

find_base:
    /* %r3 - phdrs, %r4 - phnum (not 0), %r5 - phent */
    mtctr %r4
1:
    lwz %r6, 0(%r3)
    cmplwi %r6, PT_DYNAMIC
    beq 2f
    add %r3, %r5, %r3
    bdnz 1b
    .long 0
2:
    /* base = _DYNAMIC - pt->p_vaddr */
    lwz %r3, 8(%r3)
    sub %r3, %r30, %r3
    blr

do_rel:
    /* %r3 - rel table, %r4 - rel sz (maybe 0) */
    srwi. %r4, %r4, 3
    beqlr
    mtctr %r4
1:
    lwz %r5, 4(%r3)
    andi. %r5, %r5, 0xff
    cmplwi %r5, R_PPC_RELATIVE
    bne 2f
    lwz %r5, 0(%r3)
    lwzx %r6, %r5, %r29
    add %r6, %r29, %r6
    stwx %r6, %r5, %r29
2:
    addi %r3, %r3, 8
    bdnz 1b
    blr

do_rela:
    /* %r3 - rela table, %r4 - rela sz (maybe 0) */
    cmplwi %r4, 0
    beqlr
1:
    lwz %r5, 4(%r3)
    andi. %r5, %r5, 0xff
    cmplwi %r5, R_PPC_RELATIVE
    bne 2f
    lwz %r5, 0(%r3)
    lwz %r6, 8(%r3)
    add %r6, %r29, %r6
    stwx %r6, %r5, %r29
2:
    addi %r3, %r3, 12
    addic. %r4, %r4, -12
    bne 1b
    blr

do_relr:
    /* %r3 - relr table, %r4 - relr sz (maybe 0) */
    srwi. %r4, %r4, 2
    beqlr
    addi %r3, %r3, -4
    mtctr %r4
    li %r5, 0
1:
    lwzu %r6, 4(%r3)
    andi. %r0, %r6, 1
    bne 2f
    /* base address */
    add %r5, %r6, %r29
    lwz %r6, 0(%r5)
    add %r6, %r29, %r6
    stw %r6, 0(%r5)
    addi %r5, %r5, 4
    b 3f
2:
    /* bit field */
    srwi %r6, %r6, 1
    li %r7, 31
10:
    andi. %r0, %r6, 1
    beq 11f
    lwz %r8, 0(%r5)
    add %r8, %r29, %r8
    stw %r8, 0(%r5)
11:
    addi %r5, %r5, 4
    addi %r7, %r7, -1
    bne 10b
3:
    bdnz 1b
    blr
