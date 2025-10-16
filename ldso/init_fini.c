#include "ldso.h"
#include <pthread.h>
#include <stdlib.h>

static struct dso *fini_head;
static pthread_mutex_t init_fini_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t lib_initialized = PTHREAD_COND_INITIALIZER;
static int shutting_down;
static struct dso **main_init_queue;
static struct dso *main;

struct szpair {
    size_t first;
    size_t second;
};

static struct szpair scan_vec_pair(const size_t *dynv, size_t a, size_t b)
{
    struct szpair pair = {0};
    for (const size_t *p = dynv; *p; p += 2) {
        if (*p == a)
            pair.first = p[1];
        if (*p == b)
            pair.second = p[1];
    }
    return pair;
}

hidden struct dso **__queue_initializers(struct dso *start)
{
    size_t cnt = 1;
    /* upper bound for both queue and stack is number of indirect dependencies.
     * Which I don't currently have available, so for now I am just using the module count.
     */
    for (struct dso *p = __dl_head(); p; p = p->next) cnt++;
    /* result queue and intermediate stack share an array.
     * Stack takes the end.
     */
    struct dso **queue = __libc_malloc(cnt * sizeof (struct dso *));
    if (!queue)
    {
        __dl_print_error("Out of memory for initializer queue.");
        return 0;
    }

    struct dso **qtail = queue;
    size_t sp = cnt;
    queue[--sp] = start;
    start->mark = 1;

    while (sp < cnt) {
        struct dso *p = queue[sp];
        if (!p->deps[p->mark - 1]) {
            *qtail++ = p;
            sp++;
        } else {
            struct dso *d = p->deps[p->mark - 1];
            if (!d->mark) {
                queue[--sp] = d;
                d->mark = 1;
            }
            p->mark++;
        }
    }

    for (struct dso *p = __dl_head(); p; p = p->next)
        p->mark = 0;

    *qtail = 0;
    return queue;
}

hidden void __queue_main_initializers(struct dso *argmain)
{
    main = argmain;
    main_init_queue = __queue_initializers(main);
}

static void run_init_array(void (**arr)(void), size_t sz)
{
    for (; sz; arr++, sz -= sizeof (void (*)(void)))
        (*arr)();
}

static void process_init_queue(struct dso **queue) {
    pthread_t self = __pthread_self();
    for (; *queue; queue++) {
        struct dso *p = *queue;
        while (p->initializing_thread && p->initializing_thread != self && !p->initialized)
            pthread_cond_wait(&lib_initialized, &init_fini_lock);
        if (p->initializing_thread || p->initialized)
            continue;
        p->initializing_thread = self;
        pthread_mutex_unlock(&init_fini_lock);

        size_t dyn[DT_INIT_ARRAYSZ + 1] = {0};
        __decode_vec(dyn, p->dynv, DT_INIT_ARRAYSZ + 1);
        size_t ia_off = dyn[DT_INIT_ARRAY];
        size_t ia_sz = dyn[DT_INIT_ARRAYSZ];
        if (ia_off && ia_sz)
            run_init_array((void *)(p->base + ia_off), ia_sz);

        pthread_mutex_lock(&init_fini_lock);
        p->initializing_thread = 0;
        p->initialized = 1;
        if (dyn[0] & (1 << DT_FINI_ARRAY)) {
            p->fini_next = fini_head;
            fini_head = p;
        }
        pthread_cond_broadcast(&lib_initialized);
    }
}
hidden void __run_constructors(void)
{
    pthread_mutex_lock(&init_fini_lock);
    struct szpair pia = scan_vec_pair(main->dynv, DT_PREINIT_ARRAY, DT_PREINIT_ARRAYSZ);
    if (pia.first && pia.second)
        run_init_array((void *)(main->base + pia.first), pia.second);
    process_init_queue(main_init_queue);
    __libc_free(main_init_queue);
    main_init_queue = 0;
    pthread_mutex_unlock(&init_fini_lock);
}

static int no_library_inconsistent(void)
{
    pthread_t self = __pthread_self();
    for (struct dso *p = __dl_head(); p; p = p->next)
        if (p->initializing_thread && p->initializing_thread != self)
            return 0;
    return 1;
}

hidden void __run_destructors(void)
{
    pthread_mutex_lock(&init_fini_lock);
    shutting_down = 1;
    while (!no_library_inconsistent())
        pthread_cond_wait(&lib_initialized, &init_fini_lock);

    for (struct dso *p = fini_head; p; p = p->fini_next)
    {
        pthread_mutex_unlock(&init_fini_lock);
        struct szpair fa = scan_vec_pair(p->dynv, DT_FINI_ARRAY, DT_FINI_ARRAYSZ);
        void (**fini_array)(void) = (void *)(p->base + fa.first);
        size_t n = fa.second / sizeof (void (*)(void));
        while (n--)
            fini_array[n]();
        pthread_mutex_lock(&init_fini_lock);
    }
}
