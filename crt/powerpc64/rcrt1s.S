/* relevant parts of elf.h */
#define PT_DYNAMIC  2
#define R_PPC64_RELATIVE 22
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
    addis %r2, %r12, .TOC. - _start@ha
    addi %r2, %r12, .TOC. - _start@l
    li %r3, AUX_NUM+DYN_NUM
    mr %r31,%r1
    clrrdi %r1,%r1,4
    li %r0, 0
    mtctr %r3
1:
    stdu %r0, -8(%r1)
    bdnz 1b

    addis %r30,%r12,_DYNAMIC-_start@ha
    addi %r30,%r12,_DYNAMIC-_start@l
    ld %r3, 0(%r31)    /* argc */
    sldi %r3, %r3, 3
    add %r3, %r31, %r3
    addi %r3, %r3, 8    /* r3 = just before envp */
1:
    ldu %r4, 8(%r3)
    cmpldi %r4, 0
    bne 1b

    addi %r4, %r3, 8
    mr %r3, %r1
    li %r5, AUX_NUM
    bl decode_vec
    addi %r3, %r1, AUX_NUM*8
    mr %r4, %r30
    li %r5, DYN_NUM
    bl decode_vec

    ld %r3, AT_BASE*8(%r1)
    cmpldi %r3, 0
    bne 1f
    ld %r3, AT_PHDR*8(%r1)
    ld %r4, AT_PHNUM*8(%r1)
    ld %r5, AT_PHENT*8(%r1)
    bl find_base
1:
    mr %r29, %r3
    ld %r3, (AUX_NUM+DT_REL)*8(%r1)
    ld %r4, (AUX_NUM+DT_RELSZ)*8(%r1)
    addi %r3, %r3, %r29
    bl do_rel

    ld %r3, (AUX_NUM+DT_RELA)*8(%r1)
    ld %r4, (AUX_NUM+DT_RELASZ)*8(%r1)
    addi %r3, %r3, %r29
    bl do_rela

    ld %r3, (AUX_NUM+DT_RELR)*8(%r1)
    ld %r4, (AUX_NUM+DT_RELRSZ)*8(%r1)
    addi %r3, %r3, %r29
    bl do_relr

    addi %r1, %r1, (AUX_NUM+DYN_NUM)*8-32
    mr %r3, %r31
    mr %r4, %r30
    mr %r5, %r29
    b _start_c
.size _start, . - _start

decode_vec:
/* %r3 - dst array
 * %r4 - src vector
 * %r5 - limit
 */
    ld %r6, 0(%r4)
    cmpldi %r5, 0
    beq- 3f
1:
    cmpld %r6, %r5
    bnl 2f
    ld %r7, 8(%r4)
    sldi %r6, %r6, 3
    stdx %r7, %r6, %r3
2:
    ldu %r6, 16(%r4)
    cmpldi %r6, 0
    bne 1b
3:
    blr

find_base:
    /* %r3 - phdrs, %r4 - phnum (not 0), %r5 - phent */
1:
    ld %r6, 0(%r3)
    cmplwi %r6, PT_DYNAMIC
    beq 2f
    add %r3, %r5, %r3
    addi. %r4, %r4, -1
    bne 1b
    .long 0
2:
    /* base = _DYNAMIC - pt->p_vaddr */
    ld %r3, 16(%r3)
    sub %r3, %r30, %r3
    blr

do_rel:
    /* %r3 - rel table, %r4 - rel sz (maybe 0) */
    srdi. %r4, %r4, 4
    beqlr
    mtctr %r4
1:
    ld %r5, 8(%r3)
    rlwinm %r5,%r5,0,0,31
    cmplwi %r5, R_PPC64_RELATIVE
    bne 2f
    ld %r5, 0(%r3)
    ldx %r6, %r5, %r29
    addi %r6, %r29, %r6
    stdx %r6, %r5, %r29
2:
    addi %r3, %r3, 16
    bdnz 1b
    blr

do_rela:
    /* %r3 - rela table, %r4 - rela sz (maybe 0) */
    cmpldi %r4, 0
    beqlr
1:
    ld %r5, 8(%r3)
    rlwinm %r5,%r5,0,0,31
    cmplwi %r5, R_PPC64_RELATIVE
    bne 2f
    ld %r5, 0(%r3)
    ld %r6, 16(%r3)
    addi %r6, %r29, %r6
    stdx %r6, %r5, %r29
2:
    addi %r3, %r3, 24
    addi. %r4, %r4, -24
    bne 1b
    blr

do_relr:
    /* %r3 - relr table, %r4 - relr sz (maybe 0) */
    srdi. %r4, %r4, 3
    beqlr
    addi %r3, %r3, -8
    mtctr %r4
    li %r5, 0
1:
    ldu %r6, 8(%r3)
    andi. %r0, %r6, 1
    bne 2f
    /* base address */
    addi %r5, %r6, %r29
    ld %r6, 0(%r5)
    add %r6, %r29, %r6
    std %r6, 0(%r5)
    addi %r5, %r5, 8
    b 3f
2:
    /* bit field */
    srdi %r6, %r6, 1
    li %r7, 63
10:
    andi. %r0, %r6, 1
    beq 11f
    ld %r8, 0(%r5)
    add %r8, %r29, %r8
    std %r8, 0(%r5)
11:
    addi %r5, %r5, 8
    addi %r7, %r7, -1
    bne 10b
3:
    bdnz 1b
    blr
