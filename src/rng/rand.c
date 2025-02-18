#include <stdlib.h>
#include <stdint.h>

static uint64_t seed;

int rand(void)
{
    seed = 6364136223846793005 * seed + 1;
    return seed >> 33;
}

void srand(unsigned s)
{
    seed = s;
}
