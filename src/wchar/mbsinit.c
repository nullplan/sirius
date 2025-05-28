#include <wchar.h>

int mbsinit(const mbstate_t *mbs)
{
    return mbs->__st == 0;
}
