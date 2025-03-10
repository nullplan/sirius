#include <wctype.h>

int iswdigit(wint_t x)
{
    return x - '0' < 10u;
}
