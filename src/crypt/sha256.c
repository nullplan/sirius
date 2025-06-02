#include "crypt.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* adapted from Ulrich Drepper's Public Domain implementation */
struct sha256_ctx {
    uint32_t h[8];
    size_t total;
    unsigned char buf[64];
};

/* Constants from FIPS 180-2:4.2.2 */
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void sha256_process_block(const unsigned char *buffer, struct sha256_ctx *ctx)
{
    uint32_t a = ctx->h[0];
    uint32_t b = ctx->h[1];
    uint32_t c = ctx->h[2];
    uint32_t d = ctx->h[3];
    uint32_t e = ctx->h[4];
    uint32_t f = ctx->h[5];
    uint32_t g = ctx->h[6];
    uint32_t h = ctx->h[7];

    #define ROTR32(x, c) ((x) >> (c) | (x) << (32 - (c)))
    /* Operators defined in FIPS 180-2:4.1.2 */
    #define Ch(x, y, z) ((x & y) ^ (~x & z))
    #define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
    #define S0(x) (ROTR32(x, 2) ^ ROTR32(x, 13) ^ ROTR32(x, 22))
    #define S1(x) (ROTR32(x, 6) ^ ROTR32(x, 11) ^ ROTR32(x, 25))
    #define R0(x) (ROTR32(x, 7) ^ ROTR32(x, 18) ^ (x >> 3))
    #define R1(x) (ROTR32(x, 17) ^ ROTR32(x, 19) ^ (x >> 10))

    uint32_t w[64];
    for (unsigned int i = 0; i < 16; i++)
        w[i] = (buffer[4 * i] + 0u) << 24 | buffer[4 * i + 1] << 16 | buffer[4 * i + 2] << 8 | buffer[4 * i + 3];

    for (unsigned int i = 16; i < 64; i++)
        w[i] = R1(w[i-2]) + w[i-7] + R0(w[i-15]) + w[i-16];

    for (unsigned int i = 0; i < 64; i++)
    {
        uint32_t t1 = h + S1(e) + Ch(e, f, g) + K[i] + w[i];
        uint32_t t2 = S0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;
}

static void sha256_init_ctx(struct sha256_ctx *ctx)
{
    ctx->h[0] = 0x6a09e667;
    ctx->h[1] = 0xbb67ae85;
    ctx->h[2] = 0x3c6ef372;
    ctx->h[3] = 0xa54ff53a;
    ctx->h[4] = 0x510e527f;
    ctx->h[5] = 0x9b05688c;
    ctx->h[6] = 0x1f83d9ab;
    ctx->h[7] = 0x5be0cd19;
    ctx->total = 0;
}

static void sha256_add_bytes(struct sha256_ctx *ctx, const void *data, size_t len)
{
    size_t pos = ctx->total & 63;
    const unsigned char *d = data;
    ctx->total += len;

    if (pos) {
        if (len < 64 - pos) {
            memcpy(ctx->buf + pos, data, len);
            return;
        }
        memcpy(ctx->buf + pos, d, 64 - pos);
        d += 64 - pos;
        len -= 64 - pos;
        sha256_process_block(ctx->buf, ctx);
    }

    while (len >= 64) {
        sha256_process_block(d, ctx);
        d += 64;
        len -= 64;
    }

    if (len)
        memcpy(ctx->buf, d, len);
}

static void sha256_finalize(struct sha256_ctx *ctx, unsigned char *digest)
{
    size_t pos = ctx->total & 63;
    ctx->buf[pos++] = 0x80;
    if (pos > 56) {
        memset(ctx->buf + pos, 0, 64 - pos);
        sha256_process_block(ctx->buf, ctx);
        pos = 0;
    }
    memset(ctx->buf + pos, 0, 56 - pos);
    uint64_t bits = (uint64_t)(ctx->total) << 3;
    ctx->buf[pos + 0] = bits >> 56;
    ctx->buf[pos + 1] = bits >> 48;
    ctx->buf[pos + 2] = bits >> 40;
    ctx->buf[pos + 3] = bits >> 32;
    ctx->buf[pos + 4] = bits >> 24;
    ctx->buf[pos + 5] = bits >> 16;
    ctx->buf[pos + 6] = bits >> 8;
    ctx->buf[pos + 7] = bits;
    sha256_process_block(ctx->buf, ctx);
    for (int i = 0; i < 8; i++)
    {
        digest[4 * i + 0] = ctx->h[i] >> 24;
        digest[4 * i + 1] = ctx->h[i] >> 16;
        digest[4 * i + 2] = ctx->h[i] >> 8;
        digest[4 * i + 3] = ctx->h[i];
    }
}

static void repeated_add(struct sha256_ctx *ctx, const unsigned char *hash, size_t len)
{
    while (len > 32) {
        sha256_add_bytes(ctx, hash, 32);
        len -= 32;
    }
    sha256_add_bytes(ctx, hash, len);
}

static char *sha256crypt(const char *pass, const char *salt, char *buf)
{
    char *rv = buf;
    if (memcmp(salt, "$5$", 3)) return 0;
    salt += 3;

    int rounds = 5000;
    int rounds_given = 0;
    if (!memcmp(salt, "rounds=", 7)) {
        char *endp;
        unsigned long lrounds = strtoul(salt + 7, &endp, 10);
        if (*endp == '$') {
            rounds_given = 1;
            if (lrounds < 1000)
                rounds = 1000;
            else if (lrounds >= 1000000000)
                rounds = 999999999;
            else
                rounds = lrounds;
            salt = endp + 1;
        }
    }

    size_t saltlen;
    for (saltlen = 0; salt[saltlen] && salt[saltlen] != '$' && saltlen < 16; saltlen++);

    size_t pwdlen = strlen(pass);

    struct sha256_ctx ctx;
    unsigned char hashbuf[32], dp[32], ds[32];

    sha256_init_ctx(&ctx);
    sha256_add_bytes(&ctx, pass, pwdlen);
    sha256_add_bytes(&ctx, salt, saltlen);
    sha256_add_bytes(&ctx, pass, pwdlen);
    sha256_finalize(&ctx, hashbuf);

    sha256_init_ctx(&ctx);
    sha256_add_bytes(&ctx, pass, pwdlen);
    sha256_add_bytes(&ctx, salt, saltlen);

    repeated_add(&ctx, hashbuf, pwdlen);
    for (size_t x = pwdlen; x; x >>= 1)
        if (x & 1)
            sha256_add_bytes(&ctx, hashbuf, 32);
        else
            sha256_add_bytes(&ctx, pass, pwdlen);
    sha256_finalize(&ctx, hashbuf);

    sha256_init_ctx(&ctx);
    for (size_t i = 0; i < pwdlen; i++)
        sha256_add_bytes(&ctx, pass, pwdlen);
    sha256_finalize(&ctx, dp);

    sha256_init_ctx(&ctx);
    for (size_t i = 0; i < hashbuf[0]+16; i++)
        sha256_add_bytes(&ctx, salt, saltlen);
    sha256_finalize(&ctx, ds);

    for (int i = 0; i < rounds; i++)
    {
        sha256_init_ctx(&ctx);
        if (i & 1) repeated_add(&ctx, dp, pwdlen);
        else sha256_add_bytes(&ctx, hashbuf, 32);
        if (i % 3) sha256_add_bytes(&ctx, ds, saltlen);
        if (i % 7) repeated_add(&ctx, dp, pwdlen);
        if (i & 1) sha256_add_bytes(&ctx, hashbuf, 32);
        else repeated_add(&ctx, dp, pwdlen);
        sha256_finalize(&ctx, hashbuf);
    }

    static const char alphabet[64] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    memcpy(buf, "$5$", 3);
    buf += 3;
    if (rounds_given)
        buf += sprintf(buf, "rounds=%d$", rounds);
    unsigned tmp;

    #define OUT(b2, b1, b0, trunc) do { \
        tmp = (b2) << 16 | (b1) << 8 | (b0); \
        *buf++ = alphabet[tmp & 63]; tmp >>= 6; \
        *buf++ = alphabet[tmp & 63]; tmp >>= 6; \
        *buf++ = alphabet[tmp & 63]; tmp >>= 6; \
        if (!trunc) \
            *buf++ = alphabet[tmp & 63]; \
    } while (0)

    OUT(hashbuf[0], hashbuf[10], hashbuf[20], 0);
    OUT(hashbuf[21], hashbuf[1], hashbuf[11], 0);
    OUT(hashbuf[12], hashbuf[22], hashbuf[2], 0);
    OUT(hashbuf[3], hashbuf[13], hashbuf[23], 0);
    OUT(hashbuf[24], hashbuf[4], hashbuf[14], 0);
    OUT(hashbuf[15], hashbuf[25], hashbuf[5], 0);
    OUT(hashbuf[6], hashbuf[16], hashbuf[26], 0);
    OUT(hashbuf[27], hashbuf[7], hashbuf[17], 0);
    OUT(hashbuf[18], hashbuf[28], hashbuf[8], 0);
    OUT(hashbuf[9], hashbuf[19], hashbuf[29], 0);
    OUT(0, hashbuf[31], hashbuf[30], 1);
    *buf = 0;
    return rv;
}

extern hidden char *__sha256crypt(const char *pass, const char *setting, char *buf)
{
    static const char testsalt[] = "$5$rounds=10$roundstoolow";
    static const char testpass[] = "the minimum number is still observed";
    static const char testresult[] = "$5$rounds=1000$roundstoolow$yfvwcWrQ8l/K0DAWyuPMDNHpIVlTQebY9l/gL972bIC";
    char testbuf[128];

    /* main result calculation */
    char *res = sha256crypt(pass, setting, buf);

    /* self test. The asm tells GCC that it does not know the input to the self test,
     * but that it is dependent on the output of the main calculation.
     * This increases the likelyhood of GCC not inlining the test call and using the same
     * stack spaces, which is another benefit of the test.
     */
    char *p;
    __asm__("" : "=r"(p) : "0"(testpass), "r"(res));
    char *q = sha256crypt(p, testsalt, testbuf);
    if (!q || strcmp(q, testresult)) res = 0;
    return res;
}
