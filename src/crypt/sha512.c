#include "crypt.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct sha512_ctx {
    uint64_t h[8];
    uint64_t total;
    unsigned char buf[128];
};

static const uint64_t K[80] = {
    UINT64_C(0x428a2f98d728ae22), UINT64_C(0x7137449123ef65cd),
    UINT64_C(0xb5c0fbcfec4d3b2f), UINT64_C(0xe9b5dba58189dbbc),
    UINT64_C(0x3956c25bf348b538), UINT64_C(0x59f111f1b605d019),
    UINT64_C(0x923f82a4af194f9b), UINT64_C(0xab1c5ed5da6d8118),
    UINT64_C(0xd807aa98a3030242), UINT64_C(0x12835b0145706fbe),
    UINT64_C(0x243185be4ee4b28c), UINT64_C(0x550c7dc3d5ffb4e2),
    UINT64_C(0x72be5d74f27b896f), UINT64_C(0x80deb1fe3b1696b1),
    UINT64_C(0x9bdc06a725c71235), UINT64_C(0xc19bf174cf692694),
    UINT64_C(0xe49b69c19ef14ad2), UINT64_C(0xefbe4786384f25e3),
    UINT64_C(0x0fc19dc68b8cd5b5), UINT64_C(0x240ca1cc77ac9c65),
    UINT64_C(0x2de92c6f592b0275), UINT64_C(0x4a7484aa6ea6e483),
    UINT64_C(0x5cb0a9dcbd41fbd4), UINT64_C(0x76f988da831153b5),
    UINT64_C(0x983e5152ee66dfab), UINT64_C(0xa831c66d2db43210),
    UINT64_C(0xb00327c898fb213f), UINT64_C(0xbf597fc7beef0ee4),
    UINT64_C(0xc6e00bf33da88fc2), UINT64_C(0xd5a79147930aa725),
    UINT64_C(0x06ca6351e003826f), UINT64_C(0x142929670a0e6e70),
    UINT64_C(0x27b70a8546d22ffc), UINT64_C(0x2e1b21385c26c926),
    UINT64_C(0x4d2c6dfc5ac42aed), UINT64_C(0x53380d139d95b3df),
    UINT64_C(0x650a73548baf63de), UINT64_C(0x766a0abb3c77b2a8),
    UINT64_C(0x81c2c92e47edaee6), UINT64_C(0x92722c851482353b),
    UINT64_C(0xa2bfe8a14cf10364), UINT64_C(0xa81a664bbc423001),
    UINT64_C(0xc24b8b70d0f89791), UINT64_C(0xc76c51a30654be30),
    UINT64_C(0xd192e819d6ef5218), UINT64_C(0xd69906245565a910),
    UINT64_C(0xf40e35855771202a), UINT64_C(0x106aa07032bbd1b8),
    UINT64_C(0x19a4c116b8d2d0c8), UINT64_C(0x1e376c085141ab53),
    UINT64_C(0x2748774cdf8eeb99), UINT64_C(0x34b0bcb5e19b48a8),
    UINT64_C(0x391c0cb3c5c95a63), UINT64_C(0x4ed8aa4ae3418acb),
    UINT64_C(0x5b9cca4f7763e373), UINT64_C(0x682e6ff3d6b2b8a3),
    UINT64_C(0x748f82ee5defb2fc), UINT64_C(0x78a5636f43172f60),
    UINT64_C(0x84c87814a1f0ab72), UINT64_C(0x8cc702081a6439ec),
    UINT64_C(0x90befffa23631e28), UINT64_C(0xa4506cebde82bde9),
    UINT64_C(0xbef9a3f7b2c67915), UINT64_C(0xc67178f2e372532b),
    UINT64_C(0xca273eceea26619c), UINT64_C(0xd186b8c721c0c207),
    UINT64_C(0xeada7dd6cde0eb1e), UINT64_C(0xf57d4f7fee6ed178),
    UINT64_C(0x06f067aa72176fba), UINT64_C(0x0a637dc5a2c898a6),
    UINT64_C(0x113f9804bef90dae), UINT64_C(0x1b710b35131c471b),
    UINT64_C(0x28db77f523047d84), UINT64_C(0x32caab7b40c72493),
    UINT64_C(0x3c9ebe0a15c9bebc), UINT64_C(0x431d67c49c100d4c),
    UINT64_C(0x4cc5d4becb3e42b6), UINT64_C(0x597f299cfc657e2a),
    UINT64_C(0x5fcb6fab3ad6faec), UINT64_C(0x6c44198c4a475817)
};

static void sha512_process_block(const unsigned char *buf, struct sha512_ctx *ctx)
{
    uint64_t a = ctx->h[0];
    uint64_t b = ctx->h[1];
    uint64_t c = ctx->h[2];
    uint64_t d = ctx->h[3];
    uint64_t e = ctx->h[4];
    uint64_t f = ctx->h[5];
    uint64_t g = ctx->h[6];
    uint64_t h = ctx->h[7];

    uint64_t w[80];
    #define ROTR64(x, c) ((x) >> (c) | (x) << (64 - (c)))
    #define Ch(x, y, z) ((x & y) ^ (~x & z))
    #define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
    #define S0(x) (ROTR64(x, 28) ^ ROTR64(x, 34) ^ ROTR64(x, 39))
    #define S1(x) (ROTR64(x, 14) ^ ROTR64(x, 18) ^ ROTR64(x, 41))
    #define R0(x) (ROTR64(x, 1) ^ ROTR64(x, 8) ^ (x >> 7))
    #define R1(x) (ROTR64(x, 19) ^ ROTR64(x, 61) ^ (x >> 6))

    for (int i = 0; i < 16; i++)
        w[i] = (buf[8 * i + 0] + 0ull) << 56
            | (buf[8 * i + 1] + 0ull) << 48
            | (buf[8 * i + 2] + 0ull) << 40
            | (buf[8 * i + 3] + 0ull) << 32
            | (buf[8 * i + 4] + 0ul) << 24
            | buf[8 * i + 5] << 16
            | buf[8 * i + 6] << 8
            | buf[8 * i + 7];

    for (int i = 16; i < 80; i++)
        w[i] = R1(w[i-2]) + w[i-7] + R0(w[i-15]) + w[i-16];

    for (int i = 0; i < 80; i++)
    {
        uint64_t t1 = h + S1(e) + Ch(e, f, g) + K[i] + w[i];
        uint64_t t2 = S0(a) + Maj(a, b, c);
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

static void sha512_init_ctx(struct sha512_ctx *ctx)
{
    ctx->h[0] = UINT64_C(0x6a09e667f3bcc908);
    ctx->h[1] = UINT64_C(0xbb67ae8584caa73b);
    ctx->h[2] = UINT64_C(0x3c6ef372fe94f82b);
    ctx->h[3] = UINT64_C(0xa54ff53a5f1d36f1);
    ctx->h[4] = UINT64_C(0x510e527fade682d1);
    ctx->h[5] = UINT64_C(0x9b05688c2b3e6c1f);
    ctx->h[6] = UINT64_C(0x1f83d9abfb41bd6b);
    ctx->h[7] = UINT64_C(0x5be0cd19137e2179);
    ctx->total = 0;
}

static void sha512_add_bytes(struct sha512_ctx *ctx, const void *vbuf, size_t len)
{
    const unsigned char *buf = vbuf;
    size_t pos = ctx->total & 127;
    ctx->total += len;

    if (pos) {
        if (len < 128 - pos) {
            memcpy(ctx->buf + pos, buf, len);
            return;
        }
        memcpy(ctx->buf + pos, buf, 128 - pos);
        buf += 128 - pos;
        len -= 128 - pos;
        sha512_process_block(ctx->buf, ctx);
    }

    while (len >= 128) {
        sha512_process_block(buf, ctx);
        buf += 128;
        len -= 128;
    }

    if (len)
        memcpy(ctx->buf, buf, len);
}

static void sha512_finalize(struct sha512_ctx *ctx, unsigned char *digest)
{
    size_t pos = ctx->total & 127;
    ctx->buf[pos++] = 0x80;
    if (pos > 112) {
        memset(ctx->buf + pos, 0, 128 - pos);
        sha512_process_block(ctx->buf, ctx);
        pos = 0;
    }
    memset(ctx->buf + pos, 0, 120 - pos);
    uint64_t bits = ctx->total << 3;
    ctx->buf[120] = bits >> 56;
    ctx->buf[121] = bits >> 48;
    ctx->buf[122] = bits >> 40;
    ctx->buf[123] = bits >> 32;
    ctx->buf[124] = bits >> 24;
    ctx->buf[125] = bits >> 16;
    ctx->buf[126] = bits >> 8;
    ctx->buf[127] = bits;
    sha512_process_block(ctx->buf, ctx);

    for (int i = 0; i < 8; i++) {
        digest[8 * i + 0] = ctx->h[i] >> 56;
        digest[8 * i + 1] = ctx->h[i] >> 48;
        digest[8 * i + 2] = ctx->h[i] >> 40;
        digest[8 * i + 3] = ctx->h[i] >> 32;
        digest[8 * i + 4] = ctx->h[i] >> 24;
        digest[8 * i + 5] = ctx->h[i] >> 16;
        digest[8 * i + 6] = ctx->h[i] >> 8;
        digest[8 * i + 7] = ctx->h[i];
    }
}

static void repeated_add(struct sha512_ctx *ctx, const unsigned char *hash, size_t len)
{
    while (len > 64) {
        sha512_add_bytes(ctx, hash, 64);
        len -= 64;
    }
    sha512_add_bytes(ctx, hash, len);
}
static char *sha512crypt(const char *pass, const char *salt, char *buf)
{
    char *rv = buf;
    if (memcmp(salt, "$6$", 3)) return 0;
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
    for (saltlen = 0; saltlen < 16 && salt[saltlen] && salt[saltlen] != '$'; saltlen++);
    size_t pwdlen = strlen(pass);

    struct sha512_ctx ctx;
    unsigned char hashbuf[64], dp[64], ds[64];

    sha512_init_ctx(&ctx);
    sha512_add_bytes(&ctx, pass, pwdlen);
    sha512_add_bytes(&ctx, salt, saltlen);
    sha512_add_bytes(&ctx, pass, pwdlen);
    sha512_finalize(&ctx, hashbuf);

    sha512_init_ctx(&ctx);
    sha512_add_bytes(&ctx, pass, pwdlen);
    sha512_add_bytes(&ctx, salt, saltlen);
    repeated_add(&ctx, hashbuf, pwdlen);
    for (size_t x = pwdlen; x; x >>= 1)
        if (x & 1)
            sha512_add_bytes(&ctx, hashbuf, 64);
        else
            sha512_add_bytes(&ctx, pass, pwdlen);
    sha512_finalize(&ctx, hashbuf);

    sha512_init_ctx(&ctx);
    for (size_t i = 0; i < pwdlen; i++)
        sha512_add_bytes(&ctx, pass, pwdlen);
    sha512_finalize(&ctx, dp);

    sha512_init_ctx(&ctx);
    for (size_t i = 0; i < hashbuf[0]+16; i++)
        sha512_add_bytes(&ctx, salt, saltlen);
    sha512_finalize(&ctx, ds);

    for (int i = 0; i < rounds; i++)
    {
        sha512_init_ctx(&ctx);
        if (i & 1) repeated_add(&ctx, dp, pwdlen);
        else sha512_add_bytes(&ctx, hashbuf, 64);
        if (i % 3) sha512_add_bytes(&ctx, ds, saltlen);
        if (i % 7) repeated_add(&ctx, dp, pwdlen);
        if (i & 1) sha512_add_bytes(&ctx, hashbuf, 64);
        else repeated_add(&ctx, dp, pwdlen);
        sha512_finalize(&ctx, hashbuf);
    }

    memcpy(buf, "$6$", 3);
    buf += 3;
    if (rounds_given)
        buf += sprintf(buf, "rounds=%d$", rounds);
    memcpy(buf, salt, saltlen);
    buf += saltlen;
    *buf++ = '$';

    static const char alphabet[64] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static const unsigned char permute[] = {
         0, 21, 42,
        22, 43,  1,
        44,  2, 23,
         3, 24, 45,
        25, 46,  4,
        47,  5, 26,
         6, 27, 48,
        28, 49,  7,
        50,  8, 29,
         9, 30, 51,
        31, 52, 10,
        53, 11, 32,
        12, 33, 54,
        34, 55, 13,
        56, 14, 35,
        15, 36, 57,
        37, 58, 16,
        59, 17, 38,
        18, 39, 60,
        40, 61, 19,
        62, 20, 41,
    };

    unsigned tmp;
    for (int i = 0; i < sizeof permute; i += 3)
    {
        tmp = hashbuf[permute[i]] << 16 | hashbuf[permute[i+1]] << 8 | hashbuf[permute[i+2]];
        *buf++ = alphabet[tmp & 63];
        *buf++ = alphabet[(tmp >> 6) & 63];
        *buf++ = alphabet[(tmp >> 12) & 63];
        *buf++ = alphabet[(tmp >> 18) & 63];
    }

    tmp = hashbuf[63];
    *buf++ = alphabet[tmp & 63];
    *buf++ = alphabet[(tmp >> 6) & 63];
    *buf = 0;
    return rv;
}

hidden char *__sha512crypt(const char *pass, const char *setting, char *buf)
{
    static const char testsalt[] = "$6$rounds=10$roundstoolow";
    static const char testpass[] = "the minimum number is still observed";
    static const char testresult[] = "$6$rounds=1000$roundstoolow$kUMsbe306n21p9R.FRkW3IGn.S9NPN0x50YhH1xhLsPuWGsUSklZt58jaTfF4ZEQpyUNGc0dqbpBYYBaHHrsX.";
    char testbuf[128];

    char *res = sha512crypt(pass, setting, buf);

    char *p;
    __asm__("" : "=r"(p) : "0"(testpass), "r"(res));
    char *q = sha512crypt(p, testsalt, testbuf);
    if (!q || strcmp(testbuf, testresult)) res = 0;
    return res;
}
