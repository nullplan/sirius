#include "ldso.h"
hidden void __decode_vec(size_t *arr, const size_t *vec, size_t lim)
{
    for (; *vec; vec += 2)
    {
        if (*vec < 8 * sizeof (size_t))
            *arr |= 1ul << *vec;
        if (vec[0] < lim)
            arr[vec[0]] = vec[1];
    }
}

