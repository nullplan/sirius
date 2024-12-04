typedef long long syscall_arg_t;
#define __scc(x) (sizeof ((x) + 0ULL) < 8? (unsigned long)(x) : (long long)(x))

static inline long __syscall0(long long nr)
{
    register long long rax __asm__("rax") = nr;

    __asm__ volatile("syscall" : "+r"(rax) :: "rcx","r11","memory");
    return rax;
}

static inline long __syscall1(long long nr, long long a)
{
    register long long rax __asm__("rax") = nr;
    register long long rdi __asm__("rdi") = a;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall2(long long nr, long long a, long long b)
{
    register long long rax __asm__("rax") = nr;
    register long long rdi __asm__("rdi") = a;
    register long long rsi __asm__("rsi") = b;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall3(long long nr, long long a, long long b, long long c)
{
    register long long rax __asm__("rax") = nr;
    register long long rdi __asm__("rdi") = a;
    register long long rsi __asm__("rsi") = b;
    register long long rdx __asm__("rdx") = c;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall4(long long nr, long long a, long long b, long long c, long long d)
{
    register long long rax __asm__("rax") = nr;
    register long long rdi __asm__("rdi") = a;
    register long long rsi __asm__("rsi") = b;
    register long long rdx __asm__("rdx") = c;
    register long long r10 __asm__("r10") = d;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall5(long long nr, long long a, long long b, long long c, long long d, long long e)
{
    register long long rax __asm__("rax") = nr;
    register long long rdi __asm__("rdi") = a;
    register long long rsi __asm__("rsi") = b;
    register long long rdx __asm__("rdx") = c;
    register long long r10 __asm__("r10") = d;
    register long long r8 __asm__("r8") = e;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8) : "rcx","r11","memory");
    return rax;
}

static inline long __syscall6(long long nr, long long a, long long b, long long c, long long d, long long e, long long f)
{
    register long long rax __asm__("rax") = nr;
    register long long rdi __asm__("rdi") = a;
    register long long rsi __asm__("rsi") = b;
    register long long rdx __asm__("rdx") = c;
    register long long r10 __asm__("r10") = d;
    register long long r8 __asm__("r8") = e;
    register long long r9 __asm__("r9") = f;

    __asm__ volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9) : "rcx","r11","memory");
    return rax;
}
