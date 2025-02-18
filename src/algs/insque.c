#include <search.h>

struct list {
    struct list *next, *prev;
};

void insque(void *el, void *pr)
{
    struct list *elem = el;
    struct list *pred = pr;

    elem->prev = pred;
    if (pred) {
        elem->next = pred->next;
        pred->next = elem;
    } else {
        elem->next = 0;
    }

    if (elem->next) elem->next->prev = elem;
}
