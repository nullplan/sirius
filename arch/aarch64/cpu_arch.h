static void a_crash(void) {
    __asm__("ud" ::: "memory");
    __builtin_unreachable();
}
