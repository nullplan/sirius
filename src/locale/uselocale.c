#include <locale.h>
#include <pthread.h>
#include "libc.h"

locale_t uselocale(locale_t loc)
{
    pthread_t self = __pthread_self();
    locale_t oldloc = self->locale;
    if (oldloc == &__global_locale) oldloc = LC_GLOBAL_LOCALE;
    if (loc) {
        if (loc == LC_GLOBAL_LOCALE) loc = &__global_locale;
        self->locale = loc;
    }
    return oldloc;
}
