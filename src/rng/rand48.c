#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "libm.h"

static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
static uint64_t a = 0x5deece66d;
static uint64_t x;
static uint16_t c = 13;

unsigned short *seed48(unsigned short xs[3])
{
    static unsigned short oldx[3];
    pthread_rwlock_wrlock(&lock);
    oldx[0] = x;
    oldx[1] = x >> 16;
    oldx[2] = x >> 32;
    x = xs[0] | (xs[1] + 0ul) << 16 | (xs[2] + 0ull) << 32;
    a = 0x5deece66d;
    c = 13;
    pthread_rwlock_unlock(&lock);
    return oldx;
}

void srand48(long xs)
{
    pthread_rwlock_wrlock(&lock);
    x = ((xs + 0ull) & 0xffffffff) << 16 | 0x330e;
    a = 0x5deece66d;
    c = 13;
    pthread_rwlock_unlock(&lock);
}

void lcong48(unsigned short xs[7])
{
    pthread_rwlock_wrlock(&lock);
    x = xs[0] | (xs[1] + 0ul) << 16 | (xs[2] + 0ull) << 32;
    a = xs[3] | (xs[4] + 0ul) << 16 | (xs[5] + 0ull) << 32;
    c = xs[6];
    pthread_rwlock_unlock(&lock);
}

static uint64_t rand48_step(uint64_t x)
{
    return a * x + c;
}

static uint64_t rand48_istep(void)
{
    x = rand48_step(x);
    return x;
}

static uint64_t rand48_ostep(unsigned short xs[3])
{
    uint64_t x = xs[0] | (xs[1] + 0ul) << 16 | (xs[2] + 0ull) << 32;
    pthread_rwlock_rdlock(&lock);
    x = rand48_step(x);
    pthread_rwlock_unlock(&lock);
    xs[0] = x;
    xs[1] = x >> 16;
    xs[2] = x >> 32;
    return x;
}

static double todouble(uint64_t x)
{
    return x * 0x1p-48;
}

double drand48(void)
{
    return todouble(rand48_istep());
}

double erand48(unsigned short xs[3])
{
    return todouble(rand48_ostep(xs));
}

static int to32(uint64_t x)
{
    return x >> 16;
}

long jrand48(unsigned short xs[3])
{
    return to32(rand48_ostep(xs));
}

long mrand48(void)
{
    return to32(rand48_istep());
}

static int to31(uint64_t x)
{
    return x >> 17;
}

long nrand48(unsigned short xs[3])
{
    return to31(rand48_ostep(xs));
}

long lrand48(void)
{
    return to31(rand48_istep());
}
