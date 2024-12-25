#include <string.h>
#include "libc.h"

size_t strspn(const char *str, const char *set)
{
    size_t bitset[256/8/sizeof (size_t)] = {0};
    for (const unsigned char *us = (void *)set; *us; us++) {
        BITOP(bitset, |=, *us);
    }
    const unsigned char *us;
    for (us = (void *)str; *us; us++)
        if (!BITOP(bitset, &, *us))
            break;
    return us - (const unsigned char *)str;

}
