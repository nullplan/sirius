/* need to save R1, R2, R13-R31, LR, F14-F31, VRSAVE, V20-V31.
 * Makes 23 32-bit regs, 18 64-bit FP regs, and 12 128-bit vector regs.
 * I can theoretically save the storage for the FP regs in soft-float builds.
 * Maybe later.
 */
typedef struct {
    _Alignas(16) unsigned long long __space[54];
} jmp_buf[1];
