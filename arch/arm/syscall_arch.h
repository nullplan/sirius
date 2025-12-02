#ifdef __ARMEB__
#define __SYSCALL_LL_E(x) (x) >> 32, (x)
#else
#define __SYSCALL_LL_E(x) (x), (x) >> 32
#endif
#define __SYSCALL_LL_O(x) __SYSCALL_LL_E(x)

/* in thumb mode, r7 is used as frame pointer, and can therefore not
 * be used in inline assembler.
 * To circumvent the issue, I put the syscall number into r6 there, then swap r6 with r7
 * in the lead-up to the syscall, then just restore r7 from r6 afterward,
 * and tell the compiler that the register gets updated.
 */
#ifdef __thumb__
#define __SC_NR_REG "r6"
#define __SC_NR_CLOB ,"=r"(r7)
#define __SC_CC_CLOB ,"cc" /* eor changes the condition codes */
#define __SC_INSTR "eor r7, r6; eor r6, r7; eor r7, r6; svc #0; mov r7, r6"
#else
#define __SC_NR_REG "r7"
#define __SC_NR_CLOB
#define __SC_CC_CLOB
#define __SC_INSTR "svc #0"
#endif

static inline long __syscall0(long nr)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0");
    __asm__ volatile(__SC_INSTR : "=r"(r0) __SC_NR_CLOB : "r"(r7) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

static inline long __syscall1(long nr, long a)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0") = a;
    __asm__ volatile(__SC_INSTR : "+r"(r0) __SC_NR_CLOB : "r"(r7) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

static inline long __syscall2(long nr, long a, long b)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    __asm__ volatile(__SC_INSTR : "+r"(r0) __SC_NR_CLOB : "r"(r7), "r"(r1) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

static inline long __syscall3(long nr, long a, long b, long c)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    __asm__ volatile(__SC_INSTR : "+r"(r0) __SC_NR_CLOB : "r"(r7), "r"(r1), "r"(r2) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

static inline long __syscall4(long nr, long a, long b, long c, long d)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    __asm__ volatile(__SC_INSTR : "+r"(r0) __SC_NR_CLOB : "r"(r7), "r"(r1), "r"(r2), "r"(r3) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

static inline long __syscall5(long nr, long a, long b, long c, long d, long e)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    register long r4 __asm__("r4") = e;
    __asm__ volatile(__SC_INSTR : "+r"(r0) __SC_NR_CLOB : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

static inline long __syscall6(long nr, long a, long b, long c, long d, long e, long f)
{
    register long r7 __asm__(__SC_NR_REG) = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    register long r4 __asm__("r4") = e;
    register long r5 __asm__("r5") = f;
    __asm__ volatile(__SC_INSTR : "+r"(r0) __SC_NR_CLOB : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5) : "lr", "memory" __SC_CC_CLOB);
    return r0;
}

#define ARCH_FADVISE_SWAP_ARGS
#define VDSO_CGT_NAME "__vdso_clock_gettime64"
#define VDSO_CGT_VER "LINUX_2.6"
#define VDSO_CGT_WORKAROUND
#define VDSO_CGT32_NAME "__vdso_clock_gettime"
#define VDSO_CGT32_VER "LINUX_2.6"
