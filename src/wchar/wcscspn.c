#include <wchar.h>

size_t wcscspn(const wchar_t *wcs, const wchar_t *rej)
{
    size_t i;
    for (i = 0; wcs[i]; i++)
        if (wcschr(rej, wcs[i]))
            break;
    return i;
}
