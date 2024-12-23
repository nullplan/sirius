#include "libc.h"
#include "cpu.h"
#include <string.h>
#include <stdint.h>

static uint64_t seed;
unsigned long __stack_chk_guard;

hidden size_t __next_canary(void)
{
    /* source for the multiplier: MMIX (via Wikipedia) */
    seed = 6364136223846793005 * seed + 1;

    /* on 64-bit platforms, let us overwrite one byte of the canary with 0,
     * to contain out-of-bounds string functions.
     * On 32-bit platforms, this would reduce entropy too much.
     * I will not overwrite the lowest-addressed byte so that such an event would
     * still clobber at least one byte.
     */
#ifdef __SIXTY_FOUR
    size_t rv = seed;
    ((char *)&rv)[1] = 0;
#else
    size_t rv = seed >> 32;
#endif
    return rv;
}

hidden void __init_canary(const void *entropy)
{
    if (entropy) memcpy(&seed, entropy, sizeof seed);
    else seed = (uintptr_t)__init_canary; /* let's hope for ASLR then */
    __stack_chk_guard = __next_canary();
}

void __stack_chk_fail(void)
{
    a_crash();
}
hidden void __stack_chk_fail_local(void);
weak_alias(__stack_chk_fail_local, __stack_chk_fail);
