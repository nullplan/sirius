#include <wchar.h>

size_t wcsnlen(const wchar_t *ws, size_t n)
{
    const wchar_t *end = wmemchr(ws, 0, n);
    return end? end - ws : n;
}
