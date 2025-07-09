#include "crypt.h"
#include "sha256.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static void repeated_add(struct sha256_ctx *ctx, const unsigned char *hash, size_t len)
{
    while (len > 32) {
        __sha256_add_bytes(ctx, hash, 32);
        len -= 32;
    }
    __sha256_add_bytes(ctx, hash, len);
}

static char *sha256crypt(const char *pass, const char *salt, char *buf)
{
    char *rv = buf;
    if (memcmp(salt, "$5$", 3)) return 0;
    salt += 3;

    int rounds = 5000;
    int rounds_given = 0;
    if (!memcmp(salt, "rounds=", 7)) {
        if (!isdigit(salt[7])) return 0;
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

    __sha256_init_ctx(&ctx);
    __sha256_add_bytes(&ctx, pass, pwdlen);
    __sha256_add_bytes(&ctx, salt, saltlen);
    __sha256_add_bytes(&ctx, pass, pwdlen);
    __sha256_finalize(&ctx, hashbuf);

    __sha256_init_ctx(&ctx);
    __sha256_add_bytes(&ctx, pass, pwdlen);
    __sha256_add_bytes(&ctx, salt, saltlen);

    repeated_add(&ctx, hashbuf, pwdlen);
    for (size_t x = pwdlen; x; x >>= 1)
        if (x & 1)
            __sha256_add_bytes(&ctx, hashbuf, 32);
        else
            __sha256_add_bytes(&ctx, pass, pwdlen);
    __sha256_finalize(&ctx, hashbuf);

    __sha256_init_ctx(&ctx);
    for (size_t i = 0; i < pwdlen; i++)
        __sha256_add_bytes(&ctx, pass, pwdlen);
    __sha256_finalize(&ctx, dp);

    __sha256_init_ctx(&ctx);
    for (size_t i = 0; i < hashbuf[0]+16; i++)
        __sha256_add_bytes(&ctx, salt, saltlen);
    __sha256_finalize(&ctx, ds);

    for (int i = 0; i < rounds; i++)
    {
        __sha256_init_ctx(&ctx);
        if (i & 1) repeated_add(&ctx, dp, pwdlen);
        else __sha256_add_bytes(&ctx, hashbuf, 32);
        if (i % 3) __sha256_add_bytes(&ctx, ds, saltlen);
        if (i % 7) repeated_add(&ctx, dp, pwdlen);
        if (i & 1) __sha256_add_bytes(&ctx, hashbuf, 32);
        else repeated_add(&ctx, dp, pwdlen);
        __sha256_finalize(&ctx, hashbuf);
    }

    static const char alphabet[64] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    memcpy(buf, "$5$", 3);
    buf += 3;
    if (rounds_given)
        buf += sprintf(buf, "rounds=%d$", rounds);
    memcpy(buf, salt, saltlen);
    buf += saltlen;
    *buf++ = '$';

    unsigned tmp;
    static const unsigned char permute[] = {
         0, 10, 20,
        21,  1, 11,
        12, 22,  2,
         3, 13, 23,
        24,  4, 14,
        15, 25,  5,
         6, 16, 26,
        27,  7, 17,
        18, 28,  8,
         9, 19, 29,
    };

    for (int i = 0; i < sizeof permute; i += 3)
    {
        tmp = hashbuf[permute[i]] << 16 | hashbuf[permute[i+1]] << 8 | hashbuf[permute[i+2]];
        *buf++ = alphabet[tmp & 63];
        *buf++ = alphabet[(tmp >> 6) & 63];
        *buf++ = alphabet[(tmp >> 12) & 63];
        *buf++ = alphabet[(tmp >> 18) & 63];
    }

    tmp = hashbuf[31] << 8 | hashbuf[30];
    *buf++ = alphabet[tmp & 63];
    *buf++ = alphabet[(tmp >> 6) & 63];
    *buf++ = alphabet[(tmp >> 12) & 63];
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
