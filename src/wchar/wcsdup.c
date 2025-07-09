#include <wchar.h>
#include <stdlib.h>
#include <string.h>

wchar_t *wcsdup(const wchar_t *ws)
{
    size_t l = wcslen(ws);
    wchar_t *rv = malloc((l + 1) * sizeof (wchar_t));
    return rv? memcpy(rv, ws, (l + 1) * sizeof (wchar_t)) : 0;
}
