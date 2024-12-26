#define __STARTUP_CODE
#include "libc.h"
#include <stdlib.h>
#include <elf.h>
#include "vdso.h"
#include <features.h>
#include <poll.h>
#include "cpu.h"
#include "syscall.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

static void dummy(const void *) {}
weak_alias(__init_vdso, dummy);
weak_alias(__init_canary, dummy);

static size_t dummy0(void) { return 0; }
weak_alias(__next_canary, dummy0);

static void dummy1(void) {}
weak_alias(__stdio_list_init, dummy1);

hidden int __elevated;
hidden int __thread_list_lock;
hidden unsigned long __page_size;

#define AUX_CNT 34
hidden
#ifdef __GNUC__
__attribute__((noinline))
#endif
void __init_libc(char *pn, char **envp)
{
    size_t *auxv = (void *)envp;
    while (*auxv++);
    size_t aux[AUX_CNT] = {0};
    for (; *auxv; auxv += 2) {
        if (auxv[0] < 8 * sizeof (size_t))
            aux[0] |= 1ul << auxv[0];
        if (auxv[0] < AUX_CNT)
            aux[auxv[0]] = auxv[1];
    }

    if (aux[AT_SYSINFO])
        __sysinfo = aux[AT_SYSINFO];
    __page_size = aux[AT_PAGESZ];
    struct __pthread *tp = __init_from_phdrs((void *)aux[AT_PHDR], aux[AT_PHNUM], aux[AT_PHENT]);
    tp->sysinfo = __sysinfo;
    __init_canary((void *)aux[AT_RANDOM]);
    tp->canary = __next_canary();
    tp->hwcap = aux[AT_HWCAP];
    tp->tid = __syscall(SYS_set_tid_address, &__thread_list_lock);
    __init_vdso((void *)aux[AT_SYSINFO_EHDR]);
    if ((aux[0] & 0x7800) == 0x7800 && aux[AT_UID] == aux[AT_EUID] && aux[AT_GID] == aux[AT_EGID] && !aux[AT_SECURE])
        return;

    __elevated = 1;
    struct pollfd pfd[3];
    for (int i = 0; i < 3; i++) {
        pfd[i].fd = i;
        pfd[i].events = 0;
    }
    int rv = __syscall(SYS_ppoll, pfd, 3, 0, 0, _NSIG/8);
    if (rv == -1) a_crash();
    for (int i = 0; i < 3; i++)
        if ((pfd[i].revents & POLLNVAL) && __sys_open("/dev/null", O_RDWR) != i)
            a_crash();
}

static _Noreturn void start_main_stage2(int argc, char **argv, char **envp, int (*main)(int, char **, char **))
{
    __stdio_list_init();
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

