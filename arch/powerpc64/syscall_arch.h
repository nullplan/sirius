#include <pthread.h>

static inline long __syscall0(long nr)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3");

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "=r"(r3) :: "r4","r5","r6","r7","r8","r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "=r"(r3) :: "r4","r5","r6","r7","r8","r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

static inline long __syscall1(long nr, long a)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3") = a;

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "+r"(r3) :: "r4","r5","r6","r7","r8","r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "+r"(r3) :: "r4","r5","r6","r7","r8","r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

static inline long __syscall2(long nr, long a, long b)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3") = a;
    register long r4 __asm__("r4") = b;

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "+r"(r3), "+r"(r4) :: "r5","r6","r7","r8","r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "+r"(r3), "+r"(r4) :: "r5","r6","r7","r8","r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

static inline long __syscall3(long nr, long a, long b, long c)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3") = a;
    register long r4 __asm__("r4") = b;
    register long r5 __asm__("r5") = c;

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "+r"(r3), "+r"(r4), "+r"(r5) :: "r6","r7","r8","r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "+r"(r3), "+r"(r4), "+r"(r5) :: "r6","r7","r8","r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

static inline long __syscall4(long nr, long a, long b, long c, long d)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3") = a;
    register long r4 __asm__("r4") = b;
    register long r5 __asm__("r5") = c;
    register long r6 __asm__("r6") = d;

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "+r"(r3), "+r"(r4), "+r"(r5), "+r"(r6) :: "r7","r8","r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "+r"(r3), "+r"(r4), "+r"(r5), "+r"(r6) :: "r7","r8","r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

static inline long __syscall5(long nr, long a, long b, long c, long d, long e)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3") = a;
    register long r4 __asm__("r4") = b;
    register long r5 __asm__("r5") = c;
    register long r6 __asm__("r6") = d;
    register long r7 __asm__("r7") = e;

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "+r"(r3), "+r"(r4), "+r"(r5), "+r"(r6), "+r"(r7) :: "r8","r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "+r"(r3), "+r"(r4), "+r"(r5), "+r"(r6), "+r"(r7) :: "r8","r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

static inline long __syscall6(long nr, long a, long b, long c, long d, long e, long f)
{
    register long r0 __asm__("r0") = nr;
    register long r3 __asm__("r3") = a;
    register long r4 __asm__("r4") = b;
    register long r5 __asm__("r5") = c;
    register long r6 __asm__("r6") = d;
    register long r7 __asm__("r7") = e;
    register long r8 __asm__("r8") = f;

#ifdef __STARTUP_CODE
    __asm__ volatile("sc; bns 1f; neg 3,3; 1:" : "+r"(r0), "+r"(r3), "+r"(r4), "+r"(r5), "+r"(r6), "+r"(r7), "+r"(r8) :: "r9","r10","r11","r12","cr0","ctr","memory");
#else
    register long r12 __asm__("r12") = __pthread_self()->sysinfo;
    __asm__ volatile("mtctr %1; bctrl" : "+r"(r0), "+r"(r12), "+r"(r3), "+r"(r4), "+r"(r5), "+r"(r6), "+r"(r7), "+r"(r8) :: "r9","r10","r11","lr","cr0","ctr","memory");
#endif
    return r3;
}

#define VDSO_CGT_NAME   "__kernel_clock_gettime"
#define VDSO_CGT_VER    "LINUX_2.6.15"
