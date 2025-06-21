#include "crypt.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

struct bf_ctx {
    uint32_t S[4][256];
    uint32_t P[18];
};

static const struct bf_ctx initctx = {
#include "bf_init.h"
};

static void bf_init_ctx(struct bf_ctx *ctx)
{
    *ctx = initctx;
}

static uint32_t F(const struct bf_ctx *ctx, uint32_t x)
{
    size_t a = x >> 24;
    size_t b = (x >> 16) & 0xFF;
    size_t c = (x >> 8) & 0xFF;
    size_t d = x & 0xFF;
    return ((ctx->S[0][a] + ctx->S[1][b]) ^ ctx->S[2][c]) + ctx->S[3][d];
}

static void bf_encipher(const struct bf_ctx *ctx, uint32_t *pl, uint32_t *pr)
{
    uint32_t l = *pl, r = *pr;
    for (size_t i = 0; i < 16; i += 2) {
        l ^= ctx->P[i];
        r ^= F(ctx, l);
        r ^= ctx->P[i+1];
        l ^= F(ctx, r);
    }
    l ^= ctx->P[16];
    r ^= ctx->P[17];
    *pl = r;
    *pr = l;
}

static uint32_t streamword(const unsigned char *data, size_t datalen, size_t *off)
{
    assert(*off < datalen);
    uint32_t rv = 0;
    for (size_t i = 0; i < 4; i++) {
        rv = (rv << 8) | data[*off];
        ++*off;
        if (*off >= datalen)
            *off = 0;
    }
    return rv;
}

static void bf_expand(struct bf_ctx *ctx, const uint32_t data[static 4], const uint32_t key[static 18])
{
    for (size_t i = 0; i < 18; i++)
        ctx->P[i] ^= key[i];

    size_t dataoff = 0;
    uint32_t l = 0, r = 0;
    for (size_t i = 0; i < 18; i += 2) {
        l ^= data[dataoff++];
        r ^= data[dataoff++];
        if (dataoff >= 4) dataoff = 0;
        bf_encipher(ctx, &l, &r);
        ctx->P[i] = l;
        ctx->P[i+1] = r;
    }

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 256; j += 2) {
            l ^= data[dataoff++];
            r ^= data[dataoff++];
            if (dataoff >= 4) dataoff = 0;
            bf_encipher(ctx, &l, &r);
            ctx->S[i][j] = l;
            ctx->S[i][j+1] = r;
        }
    }
}

static void bf_expand0(struct bf_ctx *ctx, const uint32_t *key, size_t keylen)
{
    assert(keylen);
    size_t keyoff = 0;
    for (size_t i = 0; i < 18; i++) {
        ctx->P[i] ^= key[keyoff++];
        if (keyoff >= keylen) keyoff = 0;
    }
    uint32_t l = 0, r = 0;
    for (size_t i = 0; i < 18; i += 2) {
        bf_encipher(ctx, &l, &r);
        ctx->P[i] = l;
        ctx->P[i+1] = r;
    }

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 256; j += 2) {
            bf_encipher(ctx, &l, &r);
            ctx->S[i][j] = l;
            ctx->S[i][j + 1] = r;
        }
}

static void bcrypt_core(const unsigned char salt[static 16], const unsigned char *key, size_t keylen, size_t rounds, unsigned char *out)
{
    /* OrpheanBeholderScryDoubt */
    uint32_t cipher[6] = {
        0x4F727068, 0x65616E42, 0x65686F6C, 0x64657253, 0x63727944, 0x6F756274
    };
    struct bf_ctx ctx;
    uint32_t saltwords[4];
    saltwords[0] = (salt[0] + 0ul) << 24 | salt[1] << 16 | salt[2] << 8 | salt[3];
    saltwords[1] = (salt[4] + 0ul) << 24 | salt[5] << 16 | salt[6] << 8 | salt[7];
    saltwords[2] = (salt[8] + 0ul) << 24 | salt[9] << 16 | salt[10] << 8 | salt[11];
    saltwords[3] = (salt[12] + 0ul) << 24 | salt[13] << 16 | salt[14] << 8 | salt[15];

    uint32_t keywords[18];
    size_t i = 0;
    keywords[0] = streamword(key, keylen, &i);
    keywords[1] = streamword(key, keylen, &i);
    keywords[2] = streamword(key, keylen, &i);
    keywords[3] = streamword(key, keylen, &i);
    keywords[4] = streamword(key, keylen, &i);
    keywords[5] = streamword(key, keylen, &i);
    keywords[6] = streamword(key, keylen, &i);
    keywords[7] = streamword(key, keylen, &i);
    keywords[8] = streamword(key, keylen, &i);
    keywords[9] = streamword(key, keylen, &i);
    keywords[10] = streamword(key, keylen, &i);
    keywords[11] = streamword(key, keylen, &i);
    keywords[12] = streamword(key, keylen, &i);
    keywords[13] = streamword(key, keylen, &i);
    keywords[14] = streamword(key, keylen, &i);
    keywords[15] = streamword(key, keylen, &i);
    keywords[16] = streamword(key, keylen, &i);
    keywords[17] = streamword(key, keylen, &i);
    bf_init_ctx(&ctx);
    bf_expand(&ctx, saltwords, keywords);
    for (size_t i = 0; i < rounds; i++) {
        bf_expand0(&ctx, keywords, 18);
        bf_expand0(&ctx, saltwords, 4);
    }

    /* now to encrypt the text 64 times */
    for (size_t i = 0; i < 64; i++)
        for (size_t j = 0; j < 6; j += 2) {
            bf_encipher(&ctx, &cipher[j], &cipher[j+1]);
        }

    /* now to output the cipher text */
    for (size_t i = 0; i < 6; i++) {
        out[4*i] = cipher[i] >> 24;
        out[4*i+1] = cipher[i] >> 16;
        out[4*i+2] = cipher[i] >> 8;
        out[4*i+3] = cipher[i];
    }
}

static const char alphabet[64] = "./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static char *base64_encode(const unsigned char *data, size_t datalen, char *out)
{
    unsigned tmp;
    while (datalen >= 3) {
        tmp = data[0] << 16 | data[1] << 8 | data[2];
        out[0] = alphabet[tmp >> 18];
        out[1] = alphabet[(tmp >> 12) & 0x3f];
        out[2] = alphabet[(tmp >> 6) & 0x3f];
        out[3] = alphabet[tmp & 0x3f];

        out += 4;
        data += 3;
        datalen -= 3;
    }
    if (datalen == 1) {
        tmp = *data;
        out[0] = alphabet[tmp >> 2];
        out[1] = alphabet[(tmp & 3) << 4];
        out += 2;
    }
    if (datalen == 2) {
        tmp = data[0] << 8 | data[1];
        out[0] = alphabet[tmp >> 10];
        out[1] = alphabet[(tmp >> 4) & 0x3f];
        out[2] = alphabet[(tmp << 2) & 0x3f];
        out += 3;
    }
    return out;
}

static int base64_decode(const char *data, unsigned char *out, size_t outlen)
{
    while (outlen >= 3) {
        unsigned tmp = 0;
        for (size_t i = 0; i < 4; i++) {
            const char *pos = memchr(alphabet, data[i], sizeof alphabet);
            if (!pos) return -1;
            tmp = (tmp << 6) | (pos - alphabet);
        }
        out[0] = tmp >> 16;
        out[1] = tmp >> 8;
        out[2] = tmp;

        data += 4;
        out += 3;
        outlen -= 3;
    }

    if (outlen == 1) {
        unsigned tmp = 0;
        for (size_t i = 0; i < 2; i++) {
            const char *pos = memchr(alphabet, data[i], sizeof alphabet);
            if (!pos) return -1;
            tmp = (tmp << 6) | (pos - alphabet);
        }
        if (tmp & 0xf) return -1;
        out[0] = tmp >> 4;
    }
    if (outlen == 2) {
        unsigned tmp = 0;
        for (size_t i = 0; i < 3; i++) {
            const char *pos = memchr(alphabet, data[i], sizeof alphabet);
            if (!pos) return -1;
            tmp = (tmp << 6) | (pos - alphabet);
        }
        if (tmp & 3) return -1;
        out[0] = tmp >> 10;
        out[1] = tmp >> 2;
    }
    return 0;
}

static char *bcrypt(const char *pass, const char *setting, char *buf)
{
    const char *salt = setting;
    if (memcmp(salt, "$2a$", 4) && memcmp(salt, "$2b$", 4))
        return 0;
    salt += 4;
    int rev = salt[-2];
    size_t pwdlen;
    if (rev == 'a')
        pwdlen = (strlen(pass) + 1) & 0xff;
    else
        pwdlen = strnlen(pass, 72) + 1;

    int logrounds;
    if (!isdigit(salt[0]) || !isdigit(salt[1]) || salt[2] != '$')
        return 0;

    logrounds = salt[0] * 10 + salt[1] - '0' * 11;
    if (logrounds < 4 || logrounds > 31) return 0;
    size_t rounds = 1ul << logrounds;

    salt += 3;
    unsigned char binsalt[16];

    if (base64_decode(salt, binsalt, sizeof binsalt)) return 0;
    salt += 22;

    unsigned char cipher[24];
    bcrypt_core(binsalt, (const unsigned char *)pass, pwdlen, rounds, cipher);

    memcpy(buf, setting, 4+3+22);
    *base64_encode(cipher, sizeof cipher - 1, buf + 4+3+22) = 0;
    return buf;
}

hidden char *__bcrypt(const char *pass, const char *setting, char *buf)
{
    static const char testpass[] = "what a beautiful day";
    static const char testhash[] = "$2b$10$zR0HUkxg5NxQHuNmAc123OHPLUQclY59lm.vVcdkRqnqh.O/xhCw.";
    char testbuf[61];

    char *result = bcrypt(pass, setting, buf);
    char *p;
    __asm__("" : "=r"(p) : "0"(testpass), "r"(result));
    char *q = bcrypt(p, testhash, testbuf);
    if (q != testbuf || memcmp(testbuf, testhash, sizeof testhash))
        result = 0;
    return result;
}
