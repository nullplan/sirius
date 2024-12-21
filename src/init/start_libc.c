#include "libc.h"
#include <stdlib.h>

hidden
#ifdef __GNUC__
__attribute__((noinline))
#endif
void __init_libc(char *pn, char **envp)
{
  /* nothing yet */
}

static _Noreturn void start_main_stage2(int argc, char **argv, char **envp, int (*main)(int, char **, char **))
{
    __run_constructors();
    exit(main(argc, argv, envp));
}

_Noreturn void __start_libc(int (*main)(int, char **, char **), long *sp)
{
    int argc = *sp;
    char **argv = (void *)(sp + 1);
    char **envp = argv + argc + 1;
    __init_libc(argv[0], envp);
    void (*stage2_func)(int, char **, char **, int (*)(int, char **, char **)) = start_main_stage2;
    __asm__ ("" : "+r"(stage2_func) :: "memory");
    stage2_func(argc, argv, envp, main);
    __builtin_unreachable();
}

