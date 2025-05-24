/* relevant parts of elf.h */
#define PT_DYNAMIC  2
#define R_AARCH64_RELATIVE 1027
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
    mov x30, sp
    and sp, x30, #-16
    mov fp, 0
    sub sp, sp, #(AUX_NUM+DYN_NUM)*8
    mov x0, #0
    mov x1, sp
    mov x2, AUX_NUM+DYN_NUM
1:
    str x0, [x1], #8
    subs x2, x2, #1
    b.ne 1b
    .hidden _DYNAMIC
    adrp x29, _DYNAMIC
    add x29, x29, #:lower12:_DYNAMIC
    ldr x1, [x30]
    add x1, x30, x1, lsl #3
    add x1, x1, #16
1:
    ldr x0, [x1], #8
    cbnz x0, 1b
    mov x0, sp
    mov x2, #AUX_NUM
    bl decode_vec

    add x0, sp, #AUX_NUM*8
    mov x1, x29
    mov x2, #DYN_NUM
    bl decode_vec

    ldr x0, [sp, AT_BASE*8]
    cbnz x0, 1f
    ldr x0, [sp, AT_PHDR*8]
    ldr x1, [sp, AT_PHNUM*8]
    ldr x2, [sp, AT_PHENT*8]
    bl find_base
1:
    mov x28, x0

    ldr x1, [sp, (AUX_NUM+DT_RELSZ)*8]
    cbz x1, 1f
    ldr x0, [sp, (AUX_NUM+DT_REL)*8]
    add x0, x0, x28
    bl do_rel
1:

    ldr x1, [sp, (AUX_NUM+DT_RELASZ)*8]
    cbz x1, 1f
    ldr x0, [sp, (AUX_NUM+DT_RELA)*8]
    add x0, x0, x28
    bl do_rela
1:

    ldr x1, [sp, (AUX_NUM+DT_RELRSZ)*8]
    cbz x1, 1f
    ldr x0, [sp, (AUX_NUM+DT_RELR)*8]
    add x0, x0, x28
    bl do_rel
1:

    add sp, sp, (AUX_NUM+DYN_NUM)*8
    mov fp, 0
    mov x0, x30
    mov x1, x29
    mov x2, x28
    b _start_c
.size _start, . - start

decode_vec:
    ldr x3, [x1]
    cbz x3, 3f
1:
    cmp x3, x2
    b.hs 2f
    ldr x4, [x1, #8]
    str x4, [x0, x3, lsl #3]
2:
    ldr x3, [x1, #16!]
    cbnz 1b
3:
    ret

find_base:
1:
    ldr w3, [x0]
    cmp w3, #PT_DYNAMIC
    b.eq 2f
    add x0, x0, x2
    subs x1, x1, #1
    b.ne 1b
    udf #0
2:
    ldr x0, [x0, #16]
    sub x0, x29, x0
    ret

do_rel:
1:
    ldr x3, [x0, #8]
    cmp w3, #R_AARCH64_RELATIVE
    b.ne 2f
    ldr x3, [x0]
    ldr x4, [x3, x28]
    add x4, x4, x28
    str x4, [x3, x28]
2:
    add x0, x0, #16
    subs x1, x1, #16
    b.ne 1b
    ret

do_rela:
1:
    ldr x3, [x0, #8]
    cmp w3, #R_AARCH64_RELATIVE
    b.ne 2f
    ldr x3, [x0]
    ldr x4, [x0, #8]
    add x4, x4, x28
    str x4, [x3, x28]
2:
    add x0, x0, #24
    subs x1, x1, #24
    b.ne 1b
    ret

do_relr:
    mov x3, #0
1:
    ldr x4, [x0], #8
    tbnz x4, #0, 2f
    add x3, x4, x28
    ldr x4, [x3]
    add x4, x4, x28
    str x4, [x3], #8
    b 3f
2:
    lsr x4, x4, #1
    mov x5, #63
10:
    tbz x4, #0, 11f
    ldr x6, [x3]
    add x6, x28, x6
    str x6, [x3]
11:
    add x3, x3, #8
    lsr x4, x4, #1
    subs x5, x5, #1
    b.ne 10b
3:
    subs x1, x1, #8
    b.ne 1b
    ret
