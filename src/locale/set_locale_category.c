#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

hidden void __set_locale_category(int cat, const char *name, struct __localedef *loc)
{
    static const char *const envname[LC_ALL] = {
        "LC_COLLATE",
        "LC_CTYPE",
        "LC_MONETARY",
        "LC_NUMERIC",
        "LC_TIME",
        "LC_MESSAGES",
    };
    assert(cat + 0u < LC_ALL + 0u);

    if (!*name) {
        name = getenv("LC_ALL");
        if (!name || !*name) name = getenv(envname[cat]);
        if (!name || !*name) name = getenv("LANG");
        if (!name || !*name) name = "C.UTF-8";
    }

    int is_posix = !strcmp(name, "C") || !strcmp(name, "POSIX");
    if (is_posix) name = "C";
    strncpy(loc->name[cat], name, NL_LANGMAX);
    loc->name[cat][NL_LANGMAX] = 0;
    if (cat == LC_CTYPE)
        loc->is_utf8 = !is_posix;
}
