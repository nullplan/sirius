#define __STARTUP_CODE
#include "tls.h"
#include <pthread.h>
#include <stdalign.h>
#include <string.h>
#include "libc.h"

static struct tls_data tls_data = {
    .size = sizeof (struct __pthread) + GAP_ABOVE_TP,
    .align = alignof (struct __pthread),
};
static size_t tls_cnt;
static struct tls_module *head, *tail;
#ifdef TLS_VARIANT_2
static size_t end_off = GAP_ABOVE_TP;
#else
static size_t end_off = sizeof (struct __pthread) + GAP_ABOVE_TP;
#endif

_Static_assert(alignof(struct __pthread) >= alignof (size_t), "");
hidden struct tls_data __get_tls_data(void) {
    struct tls_data rv = tls_data;
    if (tls_cnt)
        rv.size += (tls_cnt + 1) * sizeof (size_t);
    rv.size = (rv.size + rv.align - 1) & -rv.align;
    return rv;
}

hidden void __add_tls(struct tls_module *new)
{
    if (new->align > tls_data.align)
        tls_data.align = new->align;
#ifdef TLS_VARIANT_2
    new->off = (end_off + new->size + new->align - 1) & -new->align;
    tls_data.size += new->off - end_off;
    end_off = new->off;
#else
    new->off = (end_off + new->align - 1) & -new->align;
    tls_data.size += new->off + new->size - end_off;
    end_off = new->off + new->size;
#endif
    tls_cnt++;
    if (tail) tail->next = new;
    else head = new;
    tail = new;
}

hidden struct __pthread *__copy_tls(void *mem, size_t sz)
{
    if (!tls_cnt) return mem;

#ifdef TLS_VARIANT_2
    pthread_t tp = (void *)(((uintptr_t)mem + sz - sizeof (struct __pthread) - GAP_ABOVE_TP) & -alignof (struct __pthread));
    size_t *dtv = mem;
#else
    pthread_t tp = mem;
    size_t *dtv = (void *)(((uintptr_t)mem + sz - (tls_cnt + 1) * sizeof (size_t)) & -alignof (size_t));
#endif
    size_t i = 1;
    dtv[0] = tls_cnt;
    for (const struct tls_module *p = head; p; p = p->next)
    {
#ifdef TLS_VARIANT_2
        memcpy((char *)tp - p->off, p->image, p->len);
        dtv[i++] = (uintptr_t)tp - p->off;
#else
        memcpy((char *)tp + p->off, p->image, p->len);
        dtv[i++] = (uintptr_t)tp + p->off + DTV_OFFSET;
#endif
    }
    tp->dtv = dtv;
    return tp;
}
