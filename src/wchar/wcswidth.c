#include <wchar.h>

int wcswidth(const wchar_t *ws, size_t n)
{
    int cols = 0;
    for (size_t i = 0; i < n && ws[i]; i++) {
        int w = wcwidth(ws[i]);
        if (w == -1) return -1;
        cols += w;
    }
    return cols;
}
