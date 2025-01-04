
static inline long __syscall0(long nr)
{
    register long rax __asm__("rax") = nr;

    __asm__ volatile("syscall" : "+r"(rax) :: "rcx","r11","memory");
    return rax;
}

static inline long __syscall1(long nr, long a)
{
    register long rax __asm__("rax") = nr;
    register long rdi __asm__("rdi") = a;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall2(long nr, long a, long b)
{
    register long rax __asm__("rax") = nr;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall3(long nr, long a, long b, long c)
{
    register long rax __asm__("rax") = nr;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall4(long nr, long a, long b, long c, long d)
{
    register long rax __asm__("rax") = nr;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    register long r10 __asm__("r10") = d;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall5(long nr, long a, long b, long c, long d, long e)
{
    register long rax __asm__("rax") = nr;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    register long r10 __asm__("r10") = d;
    register long r8 __asm__("r8") = e;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall6(long nr, long a, long b, long c, long d, long e, long f)
{
    register long rax __asm__("rax") = nr;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    register long r10 __asm__("r10") = d;
    register long r8 __asm__("r8") = e;
    register long r9 __asm__("r9") = f;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9) : "rcx","r11","memory");
    return rax;
}

#define VDSO_CGT_NAME   "__vdso_clock_gettime"
#define VDSO_CGT_VER    "LINUX_2.6"
