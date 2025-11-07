#include "ldso.h"
#include "libc.h"
#include "tls.h"
#include "cpu.h"
#include "syscall.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

static const char *env_libpath; /* pointer to initial LD_LIBRARY_PATH if not elevated. Must be global to ignore future changes. */
static int ldd_mode; /* global bool for whether LDD mode is requested. */
static struct dso main;
static struct dso *head; /* head of the list of linked libraries */
static struct dso *tail; /* tail of that same list for easy appending. */
static int static_tls_cnt; /* number of TLS modules in initial load set. */
static int tls_cnt;
static int early_error;

hidden struct dso *__dl_head(void)
{
    return head;
}

hidden void __dl_push_back(struct dso *dso)
{
    tail->next = dso;
    dso->prev = tail;
    tail = dso;
}

hidden const char *__dl_libpath(void)
{
    return env_libpath;
}

hidden int __dl_static_tls_cnt(void)
{
    return static_tls_cnt;
}

hidden int __is_ldd_mode(void)
{
    return ldd_mode;
}

hidden void __dl_print_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vdprintf(2, fmt, ap);
    write(2, "\n", 1);
    early_error = 1;
    va_end(ap);
}

static void die(const char *msg)
{
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
    _Exit(1);
}

hidden void __process_dynv(struct dso *dso)
{
    for (const size_t *d = dso->dynv; *d; d += 2)
    {
        void *p = dso->base + d[1];
        switch (*d) {
            case DT_HASH:       dso->hashtab = p; break;
            case DT_GNU_HASH:   dso->ghashtab = p; break;
            case DT_SYMTAB:     dso->symtab = p; break;
            case DT_STRTAB:     dso->strtab = p; break;
        }
    }
}

static const Phdr *find_phdr_typed(const struct dso *p, int t)
{
    size_t n = p->phnum;
    for (const Phdr *ph = p->phdr; n; n--, ph = (void *)((char *)ph + p->phent))
        if (ph->p_type == t)
            return ph;
    return 0;
}

static void enumerate_phdr(struct dso *start)
{
    int cnt = 0;
    for (struct dso *p = start; p; p = p->next)
    {
        if (find_phdr_typed(p, PT_TLS))
            cnt++;
        const Phdr *ph = find_phdr_typed(p, PT_GNU_STACK);
        if (ph)
        {
            if (ph->p_memsz > __default_stacksize && ph->p_memsz < MAX_DEFAULT_STACK_SIZE)
                __default_stacksize = ph->p_memsz;
        }
    }
    if (!cnt) return;
    struct tls_module *tls_mod = __libc_calloc(cnt, sizeof (struct tls_module));
    if (!tls_mod)
    {
        __dl_print_error("Out of memory for tls module descriptions.");
        return;
    }

    for (struct dso *p = start; p; p = p->next) {
        const Phdr *ph_tls = find_phdr_typed(p, PT_TLS);
        if (!ph_tls) continue;
        p->tlsid = ++tls_cnt;
        tls_mod->size = ph_tls->p_memsz;
        tls_mod->align = ph_tls->p_align;
        tls_mod->len = ph_tls->p_filesz;
        tls_mod->image = p->base + ph_tls->p_vaddr;
        __add_tls(tls_mod);
        p->tlsoff = tls_mod->off;
        tls_mod++;
    }
}

/* At this point, relocations are processed and we have a thread pointer.
 * We still need to initialize the globals that we need for what's coming,
 * then link or load the main application, then the preloads,
 * then all the dependencies, and then perform the big relocation.
 * Not everything is initialized at the start, and we should only initialize
 * what we must, since it will all be done again once the entry point is called.
 */

#define AUX_CNT (AT_SYSINFO + 1)
hidden _Noreturn void __stage_post_reloc(long *sp, const size_t *dynv)
{
    pthread_t self = __pthread_self();
    int argc = *sp;
    char **argv = (void *)(sp + 1);
    char **envp = (void *)(argv + argc + 1);
    const size_t *auxv = (void *)envp;
    while (*auxv++);
    size_t aux[AUX_CNT] = {0};
    char *env_preload = 0; /* mutable pointer to be able to replace ':' with null bytes. Not nice but rare. */
    void *entry_point;

    __decode_vec(aux, auxv, AUX_CNT);
    self->hwcap = aux[AT_HWCAP];
    self->sysinfo = __get_sysinfo(aux);
    self->locale = &__global_locale;
    __environ = envp;
    __page_size = aux[AT_PAGESZ];
    const size_t mask = 1ul << AT_UID | 1ul << AT_EUID | 1ul << AT_GID | 1ul << AT_EGID;
    __elevated = aux[AT_SECURE] || (aux[0] & mask) != mask || aux[AT_UID] != aux[AT_EUID] || aux[AT_GID] != aux[AT_EGID];

    if (!__elevated) {
        env_libpath = getenv("LD_LIBRARY_PATH");
        env_preload = getenv("LD_PRELOAD");
    }

    /* initialize missing parts of libc object. */
    struct dso *libc = __libc_object();
    __process_dynv(libc);
    const Phdr *ph = find_phdr_typed(libc, PT_GNU_RELRO);
    if (ph) {
        libc->relro_start = ph->p_vaddr & -PAGE_SIZE;
        libc->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
    }
    __reclaim_gaps(libc);

    if (aux[AT_BASE]) {
        /* we were called as dynamic linker, so AT_PHDR points to main app. */
        main.kernel_mapped = 1;
        main.name = (void *)aux[AT_EXECFN];
        main.shortname = argv[0];
        main.phdr = (void *)aux[AT_PHDR];
        main.phent = aux[AT_PHENT];
        main.phnum = aux[AT_PHNUM];

        const Phdr *ph = find_phdr_typed(&main, PT_PHDR);
        if (ph) {
            main.base = (char *)(aux[AT_PHDR] - ph->p_vaddr);
        } else {
            ph = find_phdr_typed(&main, PT_LOAD);
            if (!ph)
                die("Main app has no load segment.");
            /* ELF spec says to find the load base by subtracting the
             * map base from the first LOAD segment's p_vaddr.
             * I don't know the map base, but I can only assume that it is
             * the page the PHDRs start on.
             */
            main.base = (char *)((aux[AT_PHDR] & -PAGE_SIZE) - ph->p_vaddr);
        }

        ph = find_phdr_typed(&main, PT_INTERP);
        if (ph)
            libc->name = main.base + ph->p_vaddr;

        ph = find_phdr_typed(&main, PT_DYNAMIC);
        if (!ph)
            die("Not a valid dynamic executable.");
        main.dynv = (void *)(main.base + ph->p_vaddr);
        __process_dynv(&main);
        entry_point = (void *)aux[AT_ENTRY];
    } else {
        /* we were called as command, so must load the app ourselves. */
        size_t len = strlen(argv[0]);
        if (len >= 3 && !strcmp(&argv[0][len - 3], "ldd"))
            ldd_mode = 1;
        else
            libc->name = argv[0];

        int optoff;
        for (optoff = 1; optoff < argc; optoff++)
        {
            if (!strcmp(argv[optoff], "--list"))
                ldd_mode = 1;
            else if (optoff < argc - 1 && !strcmp(argv[optoff], "--preload"))
                env_preload = argv[++optoff];
            else if (optoff < argc - 1 && !strcmp(argv[optoff], "--library-path"))
                env_libpath = argv[++optoff];
            else if (!strcmp(argv[optoff], "--")) {
                optoff++;
                break;
            } else
                break;
        }
        if (optoff == argc) {
            dprintf(1, "sirius loader (v0.0.1)\n");
            _Exit(0);
        }
        /* prepare stack for remaining command line */
        argv += optoff;
        argc -= optoff;
        argv[-1] = (void *)(uintptr_t)argc;

        int fd = __sys_open(argv[0], O_RDONLY);
        if (fd == -1) {
            dprintf(2, "could not load `%s': %m\n", argv[0]);
            _Exit(1);
        }
        struct stat st;
        if (fstat(fd, &st)) {
            dprintf(2, "could not stat `%s': %m\n", argv[0]);
            _Exit(1);
        }
        main.name = main.shortname = argv[0];
        main.dev = st.st_dev;
        main.ino = st.st_ino;
        Ehdr *eh = __map_library(fd, &main);
        if (!eh) _Exit(1);
        close(fd);
        entry_point = main.base + eh->e_entry;
    }

    if (!env_libpath) env_libpath = "/lib:/usr/lib";
    head = tail = &main;

    __reclaim_gaps(&main);
    if (env_preload) __load_preload(env_preload, env_libpath);
    __load_deps(head, env_libpath, 1);
    /* Have to call enumerate_phdr() before reloc_all(),
     * so the TLS relocations can be processed.
     */
    enumerate_phdr(head);
    static_tls_cnt = tls_cnt;

    /* also, process relocations of the libs *BEFORE* the main app,
     * because the main app can contain copy rels that copy stuff
     * that itself contains relocations.
     */
    __reloc_all(main.next);
    __relocate(&main);

    __queue_main_initializers(&main);

    if (early_error) _Exit(1);
    if (ldd_mode) _Exit(0);

    a_stackjmp(entry_point, argv - 1);
}
