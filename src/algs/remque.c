#include <search.h>

struct list {
    struct list *next, *prev;
};

void remque(void *el)
{
    struct list *elem = el;

    if (elem->next) elem->next->prev = elem->prev;
    if (elem->prev) elem->prev->next = elem->next;
}
