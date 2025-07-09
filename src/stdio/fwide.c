#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int fwide(FILE *f, int m)
{
    __FLOCK(f);
    if (m && !f->orientation) {
        if (m > 0) {
            f->orientation = 1;
            if (MB_CUR_MAX == 1)
                f->locale = &__c_locale;
            else
                f->locale = &__c_dot_utf8_locale;
        } else
            f->orientation = -1;
    }
    m = f->orientation;
    __FUNLOCK(f);
    return f->orientation;
}
