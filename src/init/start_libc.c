#define __STARTUP_CODE
#include "libc.h"
#include "vdso.h"
#include "cpu.h"
#include "syscall.h"
#include "futex.h"
#include "tls.h"

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/mman.h>

static void dummy(const void *p) {}
weak_alias(__init_vdso, dummy);
weak_alias(__init_canary, dummy);

static size_t dummy0(void) { return 0; }
weak_alias(__next_canary, dummy0);

static void dummy1(void) {}
weak_alias(__stdio_list_init, dummy1);
weak_alias(__init_tsd, dummy1);

static void dummy2(char *p) {}
weak_alias(__init_progname, dummy2);

hidden int __elevated;
hidden int __thread_list_lock;
hidden size_t __page_size;
hidden int __threaded;

char **__environ;
weak_alias(environ, __environ);
weak_alias(_environ, __environ);

hidden const size_t *__auxv;

hidden struct __localedef __global_locale = {{"C", "C", "C", "C", "C", "C"}, 0};

hidden size_t __hwcap;

static struct {
    struct __pthread tp;
    uintptr_t space[20];
} builtin_tls;

static void init_tp(size_t hwcap, size_t sysinfo)
{
    struct tls_data tls_data = __get_tls_data();
    char *tlsbase;
    size_t need = ((-(uintptr_t)&builtin_tls) & (tls_data.align - 1)) + tls_data.size;
    if (need <= sizeof builtin_tls) {
        tlsbase = (void *)((((uintptr_t)&builtin_tls) + tls_data.align - 1) & -tls_data.align);
    } else {
#ifdef SYS_mmap2
#undef SYS_mmap
#define SYS_mmap SYS_mmap2
#endif
        tlsbase = (void *)__syscall(SYS_mmap, 0, tls_data.size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    }
    pthread_t tp = __copy_tls(tlsbase, tls_data.size);
    tp->self = tp->next = tp->prev = tp;
    tp->sysinfo = sysinfo;
    tp->canary = __next_canary();
    tp->hwcap = hwcap;
    tp->tid = __syscall(SYS_set_tid_address, &__thread_list_lock);
    tp->locale = &__global_locale;
    if (__set_thread_area(__tp_adjust(tp)))
        a_crash();
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
    __auxv = auxv;
    size_t aux[AUX_CNT] = {0};
    for (; *auxv; auxv += 2) {
        if (auxv[0] < 8 * sizeof (size_t))
            aux[0] |= 1ul << auxv[0];
        if (auxv[0] < AUX_CNT)
            aux[auxv[0]] = auxv[1];
    }

    __page_size = aux[AT_PAGESZ];
    __init_canary((void *)aux[AT_RANDOM]);
    __init_from_phdrs((void *)aux[AT_PHDR], aux[AT_PHNUM], aux[AT_PHENT]);
    init_tp(aux[AT_HWCAP], __get_sysinfo(aux));
    __init_vdso((void *)aux[AT_SYSINFO_EHDR]);
    if (!pn || !*pn) pn = (void *)aux[AT_EXECFN];
    if (!pn) pn = "";
    __init_progname(pn);

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

