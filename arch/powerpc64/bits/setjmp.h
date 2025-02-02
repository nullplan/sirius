/* Need to save R1, R2, R13-R31, LR, CR, F14-F31, V20-V31 */
/* So that is 23 integer regs, 18 FP regs, and 12 vector regs. */
typedef struct {
    _Alignas(16) unsigned long __space[66];
} jmp_buf[1];

