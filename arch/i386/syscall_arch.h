#define __SYSCALL_LL_E(x) (x), (x) >> 32
#define __SYSCALL_LL_O(x) __SYSCALL_LL_E(x)

#define SYSCALL_INSTR   "int $0x80"

static long __syscall0(long nr)
{
    register long eax __asm__("eax") = nr;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) :: "memory");
    return eax;
}

static long __syscall1(long nr, long a)
{
    register long eax __asm__("eax") = nr;
    register long ebx __asm__("ebx") = a;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) : "r"(ebx) : "memory");
    return eax;
}

static long __syscall2(long nr, long a, long b)
{
    register long eax __asm__("eax") = nr;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) : "r"(ebx), "r"(ecx) : "memory");
    return eax;
}

static long __syscall3(long nr, long a, long b, long c)
{
    register long eax __asm__("eax") = nr;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx) : "memory");
    return eax;
}

static long __syscall4(long nr, long a, long b, long c, long d)
{
    register long eax __asm__("eax") = nr;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    register long esi __asm__("esi") = d;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi) : "memory");
    return eax;
}

static long __syscall5(long nr, long a, long b, long c, long d, long e)
{
    register long eax __asm__("eax") = nr;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    register long esi __asm__("esi") = d;
    register long edi __asm__("edi") = e;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi), "r"(edi) : "memory");
    return eax;
}

static long __syscall6(long nr, long a, long b, long c, long d, long e, long f)
{
    register long eax __asm__("eax") = nr;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    register long esi __asm__("esi") = d;
    register long edi __asm__("edi") = e;
    register long ebp __asm__("ebp") = f;

    __asm__ volatile(SYSCALL_INSTR : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi), "r"(edi), "r"(ebp) : "memory");
    return eax;
}
