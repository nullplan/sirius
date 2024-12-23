static void a_crash(void) {
    __asm__(".long 0" ::: "memory");
    __builtin_unreachable();
}
