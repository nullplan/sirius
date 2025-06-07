#include <pthread.h>
#include "cpu.h"

/* dummy C implementations that aren't used. */

hidden ptrdiff_t __tlsdesc_static(void *p)
{
    a_crash();
}

hidden ptrdiff_t __tlsdesc_dynamic(void *p)
{
    a_crash();
}
