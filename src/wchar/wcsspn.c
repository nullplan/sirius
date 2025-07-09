#include <wchar.h>

size_t wcsspn(const wchar_t *wcs, const wchar_t *acc)
{
    size_t i;
    for (i = 0; wcs[i]; i++)
        if (!wcschr(acc, wcs[i]))
            break;
    return i;
}
