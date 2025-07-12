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
#include "futex.h"

static void dummy(const void *p) {}
weak_alias(__init_vdso, dummy);
weak_alias(__init_canary, dummy);

static size_t dummy0(void) { return 0; }
weak_alias(__next_canary, dummy0);

static void dummy1(void) {}
weak_alias(__stdio_list_init, dummy1);
weak_alias(__init_tsd, dummy1);

hidden int __elevated;
hidden int __thread_list_lock;
hidden unsigned long __page_size;
hidden int __threaded;

char **__environ;
weak_alias(environ, __environ);
weak_alias(_environ, __environ);

hidden struct __localedef __global_locale;

hidden size_t __hwcap;

hidden struct __pthread *__init_tp(struct __pthread *tp, size_t hwcap, size_t sysinfo)
{
    tp->self = tp->next = tp->prev = tp;
    tp->sysinfo = sysinfo;
    tp->canary = __next_canary();
    tp->hwcap = hwcap;
    tp->tid = __syscall(SYS_set_tid_address, &__thread_list_lock);
    tp->locale = &__global_locale;
    if (__set_thread_area(__tp_adjust(tp)))
        a_crash();
    return tp;
}

#define AUX_CNT 34
hidden
#ifdef __GNUC__
__attribute__((noinline))
#endif
void __init_libc(char *pn, char **envp)
{
    __environ = envp;
    size_t *auxv = (void *)envp;
    while (*auxv++);
    size_t aux[AUX_CNT] = {0};
    for (; *auxv; auxv += 2) {
        if (auxv[0] < 8 * sizeof (size_t))
            aux[0] |= 1ul << auxv[0];
        if (auxv[0] < AUX_CNT)
            aux[auxv[0]] = auxv[1];
    }

    __page_size = aux[AT_PAGESZ];
    __init_canary((void *)aux[AT_RANDOM]);
    __init_from_phdrs((void *)aux[AT_PHDR], aux[AT_PHNUM], aux[AT_PHENT], aux[AT_HWCAP], __get_sysinfo(aux));
    __init_vdso((void *)aux[AT_SYSINFO_EHDR]);
    __global_locale = __c_locale;

    const int mask = 1<<AT_UID | 1<<AT_EUID | 1<<AT_GID | 1<<AT_EGID;
    if ((aux[0] & mask) == mask && aux[AT_UID] == aux[AT_EUID] && aux[AT_GID] == aux[AT_EGID] && !aux[AT_SECURE])
        return;

    __elevated = 1;
    struct pollfd pfd[3];
    for (int i = 0; i < 3; i++) {
        pfd[i].fd = i;
        pfd[i].events = 0;
    }
    #ifdef SYS_poll
    int rv = __syscall(SYS_poll, pfd, 3, 0);
    #else
    int rv = __syscall(SYS_ppoll, pfd, 3, ((long long[]){0, 0}), 0, _NSIG/8);
    #endif
    if (rv < 0) a_crash();
    for (int i = 0; i < 3; i++)
        if ((pfd[i].revents & POLLNVAL) && __sys_open("/dev/null", O_RDWR) != i)
            a_crash();
}

static _Noreturn void start_main_stage2(int argc, char **argv, char **envp, int (*main)(int, char **, char **))
{
    __stdio_list_init();
    __init_tsd();
    __init_sigs();
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

