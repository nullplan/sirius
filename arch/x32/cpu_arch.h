#define a_cas a_cas
static int a_cas(volatile int *p, int e, int n)
{
    __asm__("lock; cmpxchgl %2, %1" : "+a"(e), "+m"(*p) : "r"(n) : "cc");
    return e;
}

static void a_crash(void) {
    __asm__("ud2" ::: "memory");
    __builtin_unreachable();
}
