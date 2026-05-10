#include <dlfcn.h>
#include "ldso.h"
#include "tls.h"
#include "cpu.h"
#include <pthread.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

static int install_new_tls(const struct tls_module *new_mods, size_t old_tls_cnt)
{
    size_t align = _Alignof(size_t);
    size_t new_size = 0;
    for (const struct tls_module *m = new_mods; m; m = m->next) {
        new_size += m->size;
        if (m->align > align)
            align = m->align;
    }

    sigset_t ss;
    __block_usr_sigs(&ss);
    __tl_lock();
    struct __pthread *self = __pthread_self();
    size_t tls_cnt = __tls_cnt();
    struct __pthread *t = self;
    do {
        t = t->next;
        size_t *dtv = __libc_malloc((tls_cnt + 1) * sizeof (size_t));
        if (!dtv) goto cleanup;
        memcpy(dtv, t->dtv, (old_tls_cnt + 1) * sizeof (size_t));
        a_barrier();
        t->dtv = dtv;
        size_t i = 1;
        for (const struct tls_module *m = new_mods; m; m = m->next) {
            char *p = __libc_malloc(m->size); /* XXX: Use internal memalign here */
            if (!p)
                goto cleanup;
            dtv[0]++;
            dtv[old_tls_cnt + i++] = (uintptr_t)p + DTV_OFFSET;
            memcpy(p, m->image, m->len);
            memset(p + m->len, 0, m->size - m->len);
        }
        dtv += tls_cnt + 1;
    } while (t != self);
    __tl_unlock();
    __restore_sigs(&ss);
    return 0;

cleanup:
    t = self;
    do {
        t = t->next;
        for (size_t i = old_tls_cnt; i < t->dtv[0]; i++)
            __libc_free((void *)t->dtv[i+1]);
        t->dtv[0] = old_tls_cnt;
    } while (t != self);
    __tl_unlock();
    __restore_sigs(&ss);
    __dl_runtime_err("Out of memory for TLS");
    return -1;
}

void *dlopen(const char *name, int flags)
{
    struct dso *new;
    __dl_wrlock();
    struct dso *old_tail = __dl_tail();
    struct dso **initqueue = 0;
    if (!name) new = __dl_head();
    else {
        new = __load_library(name, __dl_libpath(), 0, __dl_runtime_err);
        if (new && __dl_tail() != old_tail) { /* else library was already loaded, so the deps must already have been done. */
            struct tls_state old_tls_state;
            __tls_save_state(&old_tls_state);
            if (__load_deps(new, __dl_libpath(), 0, __dl_runtime_err)
                    || __dl_enumerate_phdr(new, __dl_runtime_err)
                    || __reloc_all(new, __dl_runtime_err)
                    || !(initqueue = __queue_initializers(new, __dl_runtime_err))
                    || (__tls_cnt() != old_tls_state.tls_cnt && install_new_tls(old_tls_state.tail->next, old_tls_state.tls_cnt)))
            {
                for (struct dso *iter = old_tail->next, *next; iter; iter = next)
                {
                    next = iter->next;
                    munmap(iter->map, iter->map_len);
                    if (iter->deps && iter->deps[0])
                        __libc_free((void *)iter->deps);
                    __libc_free(iter);
                }
                __dl_restore_tail(old_tail);
                if (old_tls_state.tail)
                    __libc_free(old_tls_state.tail->next);
                __tls_restore_state(&old_tls_state);
                __libc_free(initqueue);
                new = 0;
            }
        }
    }
    for (struct dso *dso = old_tail->next; dso; dso = dso->next)
        __reclaim_gaps(dso);
    __dl_unlock();
    if (initqueue) {
        __dl_process_init_queue(initqueue);
        __libc_free(initqueue);
    }
    return new;
}
