#ifdef __ARMEB__
#define __SYSCALL_LL_E(x) (x) >> 32, (x)
#else
#define __SYSCALL_LL_E(x) (x), (x) >> 32
#endif
#define __SYSCALL_LL_O(x) __SYSCALL_LL_E(x)

/* in thumb mode, r7 is used as frame pointer, and can therefore not
 * be used in inline assembler.
 */
#ifdef __thumb__
#define __ASM____R7__
#define __do_syscall(...) __asm__ volatile("mov %1, r7; mov r7, %r2; svc 0; mov r7, %1" : "=r"(r0), "=&r"((int){0}) : __VA_ARGS__ : "memory")
#else
#define __ASM____R7__ __asm__("r7")
#define __do_syscall(...) __asm__ volatile("svc 0" : "=r"(r0) : __VA_ARGS__ : "memory")
#endif

#ifdef __thumb2__
#define __R7_OP "rI"(r7)
#else
#define __R7_OP "r"(r7)
#endif

static inline long __syscall0(long nr)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0");
    __do_syscall(__R7_OP);
    return r0;
}

static inline long __syscall1(long nr, long a)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0") = a;
    __do_syscall(__R7_OP, "r"(r0));
    return r0;
}

static inline long __syscall2(long nr, long a, long b)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    __do_syscall(__R7_OP, "r"(r0), "r"(r1));
    return r0;
}

static inline long __syscall3(long nr, long a, long b, long c)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    __do_syscall(__R7_OP, "r"(r0), "r"(r1), "r"(r2));
    return r0;
}

static inline long __syscall4(long nr, long a, long b, long c, long d)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    __do_syscall(__R7_OP, "r"(r0), "r"(r1), "r"(r2), "r"(r3));
    return r0;
}

static inline long __syscall5(long nr, long a, long b, long c, long d, long e)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    register long r4 __asm__("r4") = e;
    __do_syscall(__R7_OP, "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4));
    return r0;
}

static inline long __syscall6(long nr, long a, long b, long c, long d, long e, long f)
{
    register long r7 __ASM____R7__ = nr;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    register long r4 __asm__("r4") = e;
    register long r5 __asm__("r5") = f;
    __do_syscall(__R7_OP, "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5));
    return r0;
}

#define ARCH_FADVISE_SWAP_ARGS
#define VDSO_CGT_NAME "__vdso_clock_gettime64"
#define VDSO_CGT_VER "LINUX_2.6"
#define VDSO_CGT_WORKAROUND
#define VDSO_CGT32_NAME "__vdso_clock_gettime"
#define VDSO_CGT32_VER "LINUX_2.6"
