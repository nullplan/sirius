/* used by both sha256crypt and yescrypt. */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "sha256.h"

/* adapted from Ulrich Drepper's Public Domain implementation */
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

hidden void __sha256_init_ctx(struct sha256_ctx *ctx)
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

static void sha256_copy_ctx(struct sha256_ctx *dst, const struct sha256_ctx *src)
{
    memcpy(dst, src, offsetof(struct sha256_ctx, buf) + (src->total & 63));
}

hidden void __sha256_add_bytes(struct sha256_ctx *ctx, const void *data, size_t len)
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

hidden void __sha256_finalize(struct sha256_ctx *ctx, unsigned char *digest)
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
    ctx->buf[56] = bits >> 56;
    ctx->buf[57] = bits >> 48;
    ctx->buf[58] = bits >> 40;
    ctx->buf[59] = bits >> 32;
    ctx->buf[60] = bits >> 24;
    ctx->buf[61] = bits >> 16;
    ctx->buf[62] = bits >> 8;
    ctx->buf[63] = bits;
    sha256_process_block(ctx->buf, ctx);
    for (int i = 0; i < 8; i++)
    {
        digest[4 * i + 0] = ctx->h[i] >> 24;
        digest[4 * i + 1] = ctx->h[i] >> 16;
        digest[4 * i + 2] = ctx->h[i] >> 8;
        digest[4 * i + 3] = ctx->h[i];
    }
}

hidden void __sha256_buf(const void *data, size_t size, unsigned char *digest)
{
    struct sha256_ctx ctx;
    __sha256_init_ctx(&ctx);
    __sha256_add_bytes(&ctx, data, size);
    __sha256_finalize(&ctx, digest);
}

struct hmac_sha256_ctx {
    struct sha256_ctx ictx, octx;
};

static void hmac_sha256_init_ctx(struct hmac_sha256_ctx *ctx, const void *key, size_t klen)
{
    unsigned char padbuf[64];
    unsigned char keymd[32];
    if (klen > 64) {
        __sha256_init_ctx(&ctx->ictx);
        __sha256_add_bytes(&ctx->ictx, key, klen);
        __sha256_finalize(&ctx->ictx, keymd);
        key = keymd;
        klen = sizeof keymd;
    }
    __sha256_init_ctx(&ctx->ictx);
    __sha256_init_ctx(&ctx->octx);
    memset(padbuf, 0x36, sizeof padbuf);
    for (size_t i = 0; i < klen; i++)
        padbuf[i] ^= ((const unsigned char *)key)[i];
    __sha256_add_bytes(&ctx->ictx, padbuf, sizeof padbuf);
    for (size_t i = 0; i < sizeof padbuf; i++)
        padbuf[i] ^= 0x36 ^ 0x5c;
    __sha256_add_bytes(&ctx->octx, padbuf, sizeof padbuf);
}

static void hmac_sha256_copy_ctx(struct hmac_sha256_ctx *dst, const struct hmac_sha256_ctx *src)
{
    sha256_copy_ctx(&dst->ictx, &src->ictx);
    sha256_copy_ctx(&dst->octx, &src->octx);
}

static void hmac_sha256_add_bytes(struct hmac_sha256_ctx *ctx, const void *data, size_t datalen)
{
    __sha256_add_bytes(&ctx->ictx, data, datalen);
}

static void hmac_sha256_finalize(struct hmac_sha256_ctx *ctx, unsigned char *digest)
{
    __sha256_finalize(&ctx->ictx, digest);
    __sha256_add_bytes(&ctx->octx, digest, 32);
    __sha256_finalize(&ctx->octx, digest);
}

hidden void __hmac_sha256_buf(const void *key, size_t klen, const void *data, size_t datalen, unsigned char *digest)
{
    struct hmac_sha256_ctx ctx;
    hmac_sha256_init_ctx(&ctx, key, klen);
    hmac_sha256_add_bytes(&ctx, data, datalen);
    hmac_sha256_finalize(&ctx, digest);
}

static void pbkdf2_sha256_block(const struct hmac_sha256_ctx *pwdctx, const void *salt, size_t slen, uint64_t c, size_t block, unsigned char *buf, size_t buflen)
{
    unsigned char ubuf[32];
    unsigned char ibuf[4];
    struct hmac_sha256_ctx ctx;
    hmac_sha256_copy_ctx(&ctx, pwdctx);
    ibuf[0] = block >> 24;
    ibuf[1] = block >> 16;
    ibuf[2] = block >> 8;
    ibuf[3] = block;
    hmac_sha256_add_bytes(&ctx, salt, slen);
    hmac_sha256_add_bytes(&ctx, ibuf, 4);
    hmac_sha256_finalize(&ctx, ubuf);
    memcpy(buf, ubuf, buflen);
    for (uint64_t j = 1; j < c; j++) {
        hmac_sha256_copy_ctx(&ctx, pwdctx);
        hmac_sha256_add_bytes(&ctx, ubuf, sizeof ubuf);
        hmac_sha256_finalize(&ctx, ubuf);
        for (size_t i = 0; i < buflen; i++)
            buf[i] ^= ubuf[i];
    }
}

hidden void __pbkdf2_sha256(const void *pass, size_t pwdlen, const void *salt, size_t slen, uint64_t c, unsigned char *buf, size_t dkLen)
{
    struct hmac_sha256_ctx ctx;
    size_t block = 1;
    hmac_sha256_init_ctx(&ctx, pass, pwdlen);

    while (dkLen >= 32) {
        pbkdf2_sha256_block(&ctx, salt, slen, c, block, buf, 32);
        block++;
        buf += 32;
        dkLen -= 32;
    }
    if (dkLen)
        pbkdf2_sha256_block(&ctx, salt, slen, c, block, buf, dkLen);
}
