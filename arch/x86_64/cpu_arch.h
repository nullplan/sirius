static void a_crash(void) {
    __asm__("ud2" ::: "memory");
    __builtin_unreachable();
}
