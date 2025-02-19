#include <search.h>
#include <stdlib.h>
#include "libc.h"

struct node {
    void *data;
    struct node *left;
    struct node *right;
    int level;
};

static struct node *aa_split(struct node *n) {
    if (n && n->right && n->right->right && n->right->right->level == n->level) {
        struct node *r = n->right;
        r->level++;
        n->right = r->left;
        r->left = n;
        return r;
    }
    return n;
}

static struct node *aa_skew(struct node *n) {
    if (n && n->left && n->left->level == n->level) {
        struct node *r = n->left;
        n->left = r->right;
        r->right = n;
        return r;
    }
    return n;
}

posix_tnode *tfind(const void *key, posix_tnode *const *r, int (*cmp)(const void *, const void *))
{
    struct node *n = *r;
    while (n) {
        int c = cmp(key, n->data);
        if (!c) return n;
        if (c < 0) n = n->left;
        else n = n->right;
    }
    return 0;
}

static struct node *aa_insert(struct node *n, const void *data, int (*cmp)(const void *, const void *), void **ret)
{
    if (!n) {
        n = malloc(sizeof (struct node));
        if (n) {
            *ret = n;
            n->data = (void *)data;
            n->left = n->right = 0;
            n->level = 1;
        }
        return n;
    }
    int c = cmp(data, n->data);
    if (!c) return n;
    if (c < 0) n->left = aa_insert(n->left, data, cmp, ret);
    else n->right = aa_insert(n->right, data, cmp, ret);
    if (*ret)
        n = aa_split(aa_skew(n));
    return n;
}

posix_tnode *tsearch(const void *key, posix_tnode **pr, int (*cmp)(const void *, const void *))
{
    void *ret = 0;
    *pr = aa_insert(*pr, key, cmp, &ret);
    return ret;
}

static void aa_decrease_level(struct node *n)
{
    /* level *should* not be larger than 1 more than the min level of the children.
     * If it is, we need to decrease the level, and possibly also decrease the level of the right child.
     */
    int level_left = n->left? n->left->level : 0;
    int level_right = n->right? n->right->level : 0;
    int max = MIN(level_left, level_right) + 1;
    if (max < n->level) {
        n->level = max;
        if (max < level_right)
            n->right->level = max;
    }
}

static struct node *aa_delete_fixup(struct node *n)
{
    aa_decrease_level(n);
    n = aa_skew(n);
    n->right = aa_skew(n->right);
    if (n->right)
        n->right->right = aa_skew(n->right->right);
    n = aa_split(n);
    n->right = aa_split(n->right);
    return n;
}

static struct node *aa_delete_min(struct node *n, void **data)
{
    if (n->left) n->left = aa_delete_min(n->left, data);
    else {
        *data = n->data;
        free(n);
        return 0;
    }
    return aa_delete_fixup(n);
}

static struct node *aa_delete(struct node *n, const void *restrict key, int (*cmp)(const void *, const void *), int *deleted)
{
    if (!n) return 0;

    int c = cmp(key, n->data);
    if (!c) {
        *deleted = 1;
        /* in an Anderson tree, all singleton nodes have a right child. */
        if (!n->left) {
            struct node *rv = n->right;
            free(n);
            return rv;
        } else {
            n->right = aa_delete_min(n->right, &n->data);
        }
    } else {
        if (c < 0) n->left = aa_delete(n->left, key, cmp, deleted);
        else n->right = aa_delete(n->right, key, cmp, deleted);
    }
    if (*deleted) n = aa_delete_fixup(n);
    return n;
}

void *tdelete(const void *restrict key, posix_tnode **restrict pr, int (*cmp)(const void *, const void *))
{
    int deleted = 0;
    *pr = aa_delete(*pr, key, cmp, &deleted);
    return deleted? (void *)tdelete : 0;
}

static void inner_twalk(const struct node *n, void (*cb)(const posix_tnode *, VISIT, int), int level)
{
    if (!n) return;
    if (!n->left && !n->right) cb(n, leaf, level);
    else {
        cb(n, preorder, level);
        inner_twalk(n->left, cb, level + 1);
        cb(n, postorder, level);
        inner_twalk(n->right, cb, level + 1);
        cb(n, endorder, level);
    }
}

void twalk(const posix_tnode *r, void (*cb)(const posix_tnode *, VISIT, int))
{
    inner_twalk(r, cb, 0);
}
