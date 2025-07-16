#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cpu.h"

#ifdef __GNUC__
#define memcpy __builtin_memcpy
#define unlikely(x) __builtin_expect((x), 0)
#else
#define unlikely(x) (x)
#endif

static void cycle(void **a, size_t n, size_t sz)
{
    if (n <= 1) return;

    if (sz == 4) {
        uint32_t buf;
        memcpy(&buf, a[0], 4);
        for (size_t i = 1; i < n; i++)
            memcpy(a[i-1], a[i], 4);
        memcpy(a[n-1], &buf, 4);
    } else if (sz == 8) {
        uint64_t buf;
        memcpy(&buf, a[0], 8);
        for (size_t i = 1; i < n; i++)
            memcpy(a[i-1], a[i], 8);
        memcpy(a[n-1], &buf, 8);
    } else {
        char buf[256];
        while (unlikely(sz > sizeof buf)) {
            memcpy(buf, a[0], sizeof buf);
            for (size_t i = 1; i < n; i++) {
                memcpy(a[i - 1], a[i], sizeof buf);
                a[i - 1] = (char *)a[i - 1] + sizeof buf;
            }
            memcpy(a[n - 1], buf, sizeof buf);
            a[n - 1] = (char *)a[n - 1] + sizeof buf;
            sz -= sizeof buf;
        }
        memcpy(buf, a[0], sz);
        for (size_t i = 1; i < n; i++)
            memcpy(a[i - 1], a[i], sz);
        memcpy(a[n - 1], buf, sz);
    }
}

static void heapify(char *head, size_t order, const size_t *leo, int (*cmp)(const void *, const void *, void *), void *ctx)
{
    void *a[12 * sizeof (size_t)];
    size_t n = 1;
    a[0] = head;
    while (order > 1) {
        char *right = head - leo[0];
        char *left = right - leo[order - 2];
        if (cmp(a[0], right, ctx) >= 0 && cmp(a[0], left, ctx) >= 0) break;
        if (cmp(right, left, ctx) >= 0) {
            a[n++] = right;
            head = right;
            order -= 2;
        } else {
            a[n++] = left;
            head = left;
            order--;
        }
    }
    cycle(a, n, leo[0]);
}

static int only_one_left(const size_t p[static 2])
{
    size_t x = p[0];
    if (!x) x = p[1];
    return (x & (x - 1)) == 0;
}

static int ctz(const size_t p[static 2])
{
    if (!p[0]) return 8 * sizeof (size_t) + a_ctz(p[1]);
    return a_ctz(p[0]);
}

static int test(const size_t p[static 2], size_t x)
{
    return p[x / (8 * sizeof (size_t))] & (1ul << (x % (8 * sizeof (size_t))));
}

static void set(size_t p[static 2], size_t x)
{
    p[x / (8 * sizeof (size_t))] |= (1ul << (x % (8 * sizeof (size_t))));
}

static void clear(size_t p[static 2], size_t x)
{
    p[x / (8 * sizeof (size_t))] &= ~(1ul << (x % (8 * sizeof (size_t))));
}

static void order_roots(char *head, const size_t p[static 2], size_t order, const size_t *leo, int (*cmp)(const void *, const void *, void *), void *ctx, int known_heap)
{
    void *a[6 * sizeof (size_t) + 1];
    size_t n = 1;
    size_t l[2] = {p[0], p[1]};
    a[0] = head;
    while (!only_one_left(l)) {
        char *next = head - leo[order];
        if (cmp(next, a[0], ctx) <= 0) break;
        if (order > 1 && !known_heap) {
            char *right = head - leo[0];
            char *left = right - leo[order - 2];
            if (cmp(next, right, ctx) <= 0 || cmp(next, left, ctx) <= 0) break;
        }
        known_heap = 0;
        a[n++] = head = next;
        clear(l, order);
        order = ctz(l);
    }
    if (!known_heap) {
        cycle(a, n, leo[0]);
        heapify(head, order, leo, cmp, ctx);
    }
}

hidden void __qsort_r(void *base, size_t n, size_t sz, int (*cmp)(const void *, const void *, void *), void *ctx)
{
    if (n <= 1) return;
    size_t p[2] = {3, 0};
    size_t order = 0;
    char *head = (char *)base + sz;

    size_t leo[12 * sizeof(size_t)];
    leo[0] = leo[1] = sz;
    for (size_t *lp = leo + 1; *lp < n * sz; lp[1] = lp[0] + lp[-1] + sz, lp++);

    char *end = (char *)base + n * sz;
    while (head < end - sz) {
        /* 1st optimization: We only need to call order_roots(),
         * if the node will end up as a root in the final heap.
         * It will only end up a root if it becomes neither a
         * left child nor a right child.
         *
         * The node becomes a right child if any elements are left
         * to be added and the smallest two trees are of consecutive size.
         * The node becomes a left child if it does not become a right
         * child and more data is left to add than needed to fill up the
         * right child tree.
         */
        if (test(p, order + 1)) {
            heapify(head, order, leo, cmp, ctx);
            clear(p, order);
            clear(p, order + 1);
            set(p, order + 2);
            order += 2;
        } else {
            if (end - head - sz > leo[order - 1])
                heapify(head, order, leo, cmp, ctx);
            else
                order_roots(head, p, order, leo, cmp, ctx, 0);
            if (order == 1) order = 0;
            else order = 1;
            set(p, order);
        }
        head += sz;
    }
    order_roots(head, p, order, leo, cmp, ctx, 0);

    while (head > (char *)base + sz) {
        head -= sz;
        clear(p, order);
        if (order == 0)
            order = 1;
        else if (order == 1)
            order = ctz(p);
        else {
            set(p, order - 1);
            order_roots(head - leo[order - 2], p, order - 1, leo, cmp, ctx, 1);
            set(p, order - 2);
            order_roots(head, p, order - 2, leo, cmp, ctx, 1);
            order -= 2;
        }
    }
}
weak_alias(qsort_r, __qsort_r);
