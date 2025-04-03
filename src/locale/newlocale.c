#include <locale.h>
#include <stdlib.h>
#include <string.h>

locale_t newlocale(int mask, const char *name, locale_t loc)
{
    struct __localedef temp;
    for (int i = 0; i < LC_ALL; i++)
    {
        const char *catname = loc && !(mask & (1 << i))? loc->name[i] :
            (mask & (1 << i))? name : "";
        __set_locale_category(i, catname, &temp);
    }

    if (!loc) {
        if (!memcmp(&temp, &__c_locale, sizeof (struct __localedef))) return &__c_locale;
        if (!memcmp(&temp, &__c_dot_utf8_locale, sizeof (struct __localedef))) return &__c_dot_utf8_locale;

        loc = malloc(sizeof (struct __localedef));
    }

    if (loc) {
        memcpy((void *)loc, &temp, sizeof (struct __localedef));
    }
    return loc;
}
