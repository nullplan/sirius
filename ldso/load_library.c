#include "ldso.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>

static struct dso *const no_deps[1] = {0};
static struct dso *builtin_deps[2];

static const char *const libc_alias[] = {
    "c", "pthread", "m", "rt", "xnet", "dl", "util"
};

static struct dso *load_libc(int variant)
{
    struct dso *libc = __libc_object();
    if (__is_ldd_mode()) {
        static unsigned reported;
        if (!(reported & (1u << variant))) {
            reported |= 1u << variant;
            dprintf(1, "\tlib%s.so => %s (%p)\n", libc_alias[variant], libc->name, (void *)libc->base);
        }
    }

    if (!libc->prev)
        __dl_push_back(libc);
    return libc;
}

static int path_open(const char *name, const char *search_path, char *namebuf, size_t namesize)
{
    const char *a, *z;
    size_t namelen = strlen(name);
    if (*search_path) {
        a = search_path;
        for (;;) {
            z = a + __stridx(a, ':');
            if (z - a + 2 + namelen < namesize) {
                if (a == z)
                    memcpy(namebuf, name, namelen + 1);
                else {
                    memcpy(namebuf, a, z - a);
                    namebuf[z-a] = '/';
                    memcpy(namebuf + (z-a) + 1, name, namelen + 1);
                }
                int fd = open(namebuf, O_RDONLY | O_CLOEXEC);
                if (fd != -1) return fd;
            }
            if (!*z) break;
            a = z + 1;
        }
    }
    errno = ENOENT;
    return -1;
}

hidden struct dso *__load_library(const char *name, const char *search_path, int at_startup)
{
    if (!*name) {
        errno = EINVAL;
        return 0;
    }

    if (!strncmp(name, "lib", 3)) {
        for (size_t i = 0; i < sizeof libc_alias / sizeof *libc_alias; i++) {
            size_t l = strlen(libc_alias[i]);
            if (!strncmp(name + 3, libc_alias[i], l) && name[3+l] == '.') {
                return load_libc(i);
            }
        }
    }

    const char *fullname;
    char namebuf[2 * NAME_MAX + 2];
    int fd;
    if (strchr(name, '/')) {
        fullname = name;
        fd = open(name, O_RDONLY | O_CLOEXEC);
    } else {
        if (strnlen(name, NAME_MAX + 1) > NAME_MAX)
            return 0;

        for (struct dso *dso = __dl_head(); dso; dso = dso->next)
            if (dso->shortname && !strcmp(dso->shortname, name))
                return dso;

        fullname = namebuf;
        fd = path_open(name, search_path, namebuf, sizeof namebuf);
    }

    if (fd == -1) {
        __dl_print_error("Error loading `%s': %m", name);
        return 0;
    }
    struct stat st;
    if (fstat(fd, &st)) {
        __dl_print_error("Error stating `%s': %m", name);
        close(fd);
        return 0;
    }

    for (struct dso *dso = __dl_head(); dso; dso = dso->next)
        if (!dso->kernel_mapped && dso->dev == st.st_dev && dso->ino == st.st_ino)
        {
            /* if library was previously linked in by pathname and is now referred to
             * by shortname, then assign the shortname.
             *
             * And if that fails, then so be it. I can't deal with every problem.
             *
             * At startup time, the name provided is a DT_NEEDED entry of a library
             * that will never be unloaded, so can be used directly. At runtime,
             * it is provided by the caller and not guaranteed to live any longer than
             * the call itself.
             */
            if (!dso->shortname && fullname != name)
            {
                dso->shortname = at_startup? name : strdup(name);
                if (__is_ldd_mode() && dso->shortname)
                    dprintf(1, "\t%s%s%s (%p)\n", dso->shortname? dso->shortname : "", dso->shortname? " => " : "", dso->name, (void *)dso->base);
            }
            close(fd);
            return dso;
        }

    struct dso temp_dso = {0};
    temp_dso.name = fullname;
    void *map = __map_library(fd, &temp_dso);
    close(fd);
    if (!map) return 0;

    if (__dl_find_sym("stdin", &temp_dso, 1).sym && __dl_find_sym("_Exit", &temp_dso, 1).sym)
    {
        munmap(temp_dso.map, temp_dso.map_len);
        return load_libc(0);
    }

    struct dso *dso = __libc_malloc(sizeof (struct dso) + strlen(fullname) + 1);
    if (!dso) {
        __dl_print_error("Error loading `%s': Out of memory", name);
        munmap(temp_dso.map, temp_dso.map_len);
        return 0;
    }

    *dso = temp_dso;
    dso->name = (void *)(dso + 1);
    dso->dev = st.st_dev;
    dso->ino = st.st_ino;
    strcpy((char *)dso->name, fullname);
    if (fullname != name) {
        dso->shortname = strrchr(dso->name, '/');
        if (dso->shortname) dso->shortname++;
        else dso->shortname = dso->name;
    }
    __dl_push_back(dso);

    if (__is_ldd_mode()) dprintf(1, "\t%s%s%s (%p)\n", dso->shortname? dso->shortname : "", dso->shortname? " => " : "", dso->name, (void *)dso->base);

    /* if we are at startup, this library will not be unmapped again.
     * If an error occurs, we will exit.
     * So the gaps can be donated to malloc safely.
     * At runtime, this can only happen after relocation has happened successfully.
     */
    if (at_startup)
        __reclaim_gaps(dso);
    return dso;
}

hidden void __load_preload(char *preload, const char *search_path)
{
    char *a, *z;
    a = preload;
    for (;;)
    {
        z = a + __stridx(a, ':');
        int cont = *z;
        *z = 0;
        __load_library(a, search_path, 1);
        if (!cont) break;
        *z = ':';
        a = z + 1;
    }
}

static void load_direct_deps(struct dso *dso, const char *search_path, int at_startup, int is_main)
{
    if (dso->deps) return;
    size_t cnt = 0;
    /* preload libraries are pseudo-deps of the main application
     * for the purposes of initializing and stuff.
     */
    if (is_main) {
        for (struct dso *ld = dso->next; ld; ld = ld->next)
            cnt++;
    }
    for (const size_t *i = dso->dynv; *i; i += 2)
        if (*i == DT_NEEDED)
            cnt++;

    if (!cnt) {
        dso->deps = no_deps;
        return;
    }

    struct dso **deps;
    if (is_main && cnt < 2)
        deps = builtin_deps;
    else {
        deps = __libc_calloc(cnt + 1, sizeof (struct dso *));
        if (!deps) {
            __dl_print_error("Error loading `%s': Could not allocate space for dependencies.", dso->name);
            return;
        }
    }

    cnt = 0;
    if (is_main)
        for (struct dso *p = dso->next; p; p = p->next)
            deps[cnt++] = p;
    for (const size_t *i = dso->dynv; *i; i += 2)
        if (i[0] == DT_NEEDED)
            deps[cnt++] = __load_library(dso->strtab + i[1], search_path, at_startup);
    deps[cnt] = 0;
    dso->deps = deps;
}

hidden void __load_deps(struct dso *dso, const char *search_path, int at_startup)
{
    /* we are called on the main application exactly once at startup,
     * meaning if at_startup == 1, then dso points to the main app handle.
     * So we can tell load_direct_deps exactly that for the first iteration.
     */
    int is_main = at_startup;
    for (; dso; dso = dso->next)
    {
        load_direct_deps(dso, search_path, at_startup, is_main);
        is_main = 0;
    }
}

