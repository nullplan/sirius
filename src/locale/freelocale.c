#include <locale.h>
#include <stdlib.h>

void freelocale(locale_t loc)
{
    if (loc != &__c_locale && loc != &__c_dot_utf8_locale)
        free((void *)loc);
}
