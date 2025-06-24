#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

static uint32_t init[32] = {
    0x00000000, 0x5851f42d, 0xc0b18ccf, 0xcbb5f646,
    0xc7033129, 0x30705b04, 0x20fd5db4, 0x9a8b7f78,
    0x502959d8, 0xab894868, 0x6c0356a7, 0x88cdb7ff,
    0xb477d43f, 0x70a3a52b, 0xa8e4baf1, 0xfd8341fc,
    0x8ae16fd9, 0x742d2f7a, 0x0d1f0796, 0x76035e09,
    0x40f7702c, 0x6fa72ca5, 0xaaa84157, 0x58a0df74,
    0xc74a0364, 0xae533cc4, 0x04185faf, 0x6de3b115,
    0x0cab8628, 0xf043bfa4, 0x398150e9, 0x37521657,
};
static uint32_t *start = init + 1;
static size_t wi = 3;
static size_t ri = 0;
static size_t max = 31;
static struct lock lock;

static uint64_t lcg64(uint64_t x)
{
    return 6364136223846793005 * x + 1;
}

static uint32_t lcg32(uint32_t x)
{
    return 1103515245 * x + 12345;
}

static void srandom_unlocked(unsigned seed)
{
    if (max == 0)
        start[0] = seed;
    else {
        uint64_t s = seed;
        for (size_t i = 0; i < max; i++) {
            s = lcg64(s);
            start[i] = s >> 32;
        }
        start[0] |= 1; /* ensure at least one odd number in the array */
    }
}

char *initstate(unsigned seed, char *state, size_t size)
{
    if (size < 8) return 0;
    __lock(&lock);
    char *rv = (void *)(start - 1);
    start[-1] = max << 16 | wi << 8 | ri;
    start = (uint32_t *)state + 1;
    if (size < 32)
        max = 0;
    else if (size < 64)
        max = 7;
    else if (size < 128)
        max = 15;
    else if (size < 256)
        max = 31;
    else
        max = 63;
    ri = 0;
    wi = max == 7 || max == 31? 3 : 1;
    srandom_unlocked(seed);
    __unlock(&lock);
    return rv;
}

char *setstate(char *state)
{
    __lock(&lock);
    start[-1] = max << 16 | wi << 8 | ri;
    char *rv = (void *)(start - 1);
    start = (uint32_t *)state + 1;
    max = start[-1] >> 16;
    wi = (start[-1] >> 8) & 0xFF;
    ri = start[-1] & 0xFF;
    __unlock(&lock);
    return rv;
}

long random(void)
{
    __lock(&lock);
    long rv;
    if (max == 0) {
        start[0] = lcg32(start[0]);
        rv = start[0] >> 1;
    } else {
        start[wi] += start[ri];
        rv = start[wi] >> 1;
        wi++;
        ri++;
        if (wi >= max) wi = 0;
        if (ri >= max) ri = 0;
    }
    __unlock(&lock);
    return rv;
}

void srandom(unsigned seed)
{
    __lock(&lock);
    srandom_unlocked(seed);
    __unlock(&lock);
}
