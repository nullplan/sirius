#include "crypt.h"
#include <stdint.h>
#include <string.h>

struct md5ctx {
    uint32_t h[4];
    size_t pos;
    unsigned char buf[64];
};

static void md5_process_block(struct md5ctx *ctx, const unsigned char *buf)
{
    uint32_t a, b, c, d, x[16];
    a = ctx->h[0];
    b = ctx->h[1];
    c = ctx->h[2];
    d = ctx->h[3];

    for (size_t i = 0; i < 16; i ++)
        x[i] = buf[4*i] | buf[4*i+1] << 8 | buf[4*i+2] << 16 | (buf[4*i+3] + 0ul) << 24;

    #define ROTL32(x, c) ((x) << (c) | (x) >> (32 - (c)))
    #define F(x,y,z) (((x) & (y)) | (~(x) & (z)))
    #define G(x,y,z) (((x) & (z)) | ((y) & ~(z)))
    #define H(x,y,z) ((x) ^ (y) ^ (z))
    #define I(x,y,z) ((y) ^ ((x) | ~(z)))

    #define S11         7
    #define S12         12
    #define S13         17
    #define S14         22
    #define S21         5
    #define S22         9
    #define S23         14
    #define S24         20
    #define S31         4
    #define S32         11
    #define S33         16
    #define S34         23
    #define S41         6
    #define S42         10
    #define S43         15
    #define S44         21

    #define FF(a,b,c,d,x,s,ac) (a) = ROTL32(a + F(b,c,d) + (x) + (ac), (s)) + (b)
    #define GG(a,b,c,d,x,s,ac) (a) = ROTL32(a + G(b,c,d) + (x) + (ac), (s)) + (b)
    #define HH(a,b,c,d,x,s,ac) (a) = ROTL32(a + H(b,c,d) + (x) + (ac), (s)) + (b)
    #define II(a,b,c,d,x,s,ac) (a) = ROTL32(a + I(b,c,d) + (x) + (ac), (s)) + (b)

    FF(a, b, c, d, x[ 0], S11, 0xd76aa478);
    FF(d, a, b, c, x[ 1], S12, 0xe8c7b756);
    FF(c, d, a, b, x[ 2], S13, 0x242070db);
    FF(b, c, d, a, x[ 3], S14, 0xc1bdceee);
    FF(a, b, c, d, x[ 4], S11, 0xf57c0faf);
    FF(d, a, b, c, x[ 5], S12, 0x4787c62a);
    FF(c, d, a, b, x[ 6], S13, 0xa8304613);
    FF(b, c, d, a, x[ 7], S14, 0xfd469501);
    FF(a, b, c, d, x[ 8], S11, 0x698098d8);
    FF(d, a, b, c, x[ 9], S12, 0x8b44f7af);
    FF(c, d, a, b, x[10], S13, 0xffff5bb1);
    FF(b, c, d, a, x[11], S14, 0x895cd7be);
    FF(a, b, c, d, x[12], S11, 0x6b901122);
    FF(d, a, b, c, x[13], S12, 0xfd987193);
    FF(c, d, a, b, x[14], S13, 0xa679438e);
    FF(b, c, d, a, x[15], S14, 0x49b40821);
    GG(a, b, c, d, x[ 1], S21, 0xf61e2562);
    GG(d, a, b, c, x[ 6], S22, 0xc040b340);
    GG(c, d, a, b, x[11], S23, 0x265e5a51);
    GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
    GG(a, b, c, d, x[ 5], S21, 0xd62f105d);
    GG(d, a, b, c, x[10], S22,  0x2441453);
    GG(c, d, a, b, x[15], S23, 0xd8a1e681);
    GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
    GG(a, b, c, d, x[ 9], S21, 0x21e1cde6);
    GG(d, a, b, c, x[14], S22, 0xc33707d6);
    GG(c, d, a, b, x[ 3], S23, 0xf4d50d87);
    GG(b, c, d, a, x[ 8], S24, 0x455a14ed);
    GG(a, b, c, d, x[13], S21, 0xa9e3e905);
    GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8);
    GG(c, d, a, b, x[ 7], S23, 0x676f02d9);
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);
    HH(a, b, c, d, x[ 5], S31, 0xfffa3942);
    HH(d, a, b, c, x[ 8], S32, 0x8771f681);
    HH(c, d, a, b, x[11], S33, 0x6d9d6122);
    HH(b, c, d, a, x[14], S34, 0xfde5380c);
    HH(a, b, c, d, x[ 1], S31, 0xa4beea44);
    HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9);
    HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60);
    HH(b, c, d, a, x[10], S34, 0xbebfbc70);
    HH(a, b, c, d, x[13], S31, 0x289b7ec6);
    HH(d, a, b, c, x[ 0], S32, 0xeaa127fa);
    HH(c, d, a, b, x[ 3], S33, 0xd4ef3085);
    HH(b, c, d, a, x[ 6], S34,  0x4881d05);
    HH(a, b, c, d, x[ 9], S31, 0xd9d4d039);
    HH(d, a, b, c, x[12], S32, 0xe6db99e5);
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8);
    HH(b, c, d, a, x[ 2], S34, 0xc4ac5665);
    II(a, b, c, d, x[ 0], S41, 0xf4292244);
    II(d, a, b, c, x[ 7], S42, 0x432aff97);
    II(c, d, a, b, x[14], S43, 0xab9423a7);
    II(b, c, d, a, x[ 5], S44, 0xfc93a039);
    II(a, b, c, d, x[12], S41, 0x655b59c3);
    II(d, a, b, c, x[ 3], S42, 0x8f0ccc92);
    II(c, d, a, b, x[10], S43, 0xffeff47d);
    II(b, c, d, a, x[ 1], S44, 0x85845dd1);
    II(a, b, c, d, x[ 8], S41, 0x6fa87e4f);
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0);
    II(c, d, a, b, x[ 6], S43, 0xa3014314);
    II(b, c, d, a, x[13], S44, 0x4e0811a1);
    II(a, b, c, d, x[ 4], S41, 0xf7537e82);
    II(d, a, b, c, x[11], S42, 0xbd3af235);
    II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb);
    II(b, c, d, a, x[ 9], S44, 0xeb86d391);

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
}

static void md5_init_ctx(struct md5ctx *ctx)
{
    ctx->h[0] = 0x67452301;
    ctx->h[1] = 0xefcdab89;
    ctx->h[2] = 0x98badcfe;
    ctx->h[3] = 0x10325476;
    ctx->pos = 0;
}


static void md5_add_bytes(struct md5ctx *ctx, const void *vd, size_t len)
{
    size_t pos = ctx->pos & 63;
    const unsigned char *d = vd;
    ctx->pos += len;
    if (pos) {
        if (len < 64 - pos) {
            memcpy(ctx->buf + pos, vd, len);
            return;
        }
        memcpy(ctx->buf + pos, d, 64 - pos);
        d += 64 - pos;
        len -= 64 - pos;
        md5_process_block(ctx, ctx->buf);
    }

    while (len >= 64) {
        md5_process_block(ctx, d);
        d += 64;
        len -= 64;
    }

    if (len)
        memcpy(ctx->buf, d, len);
}

static void md5_finalize(struct md5ctx *ctx, unsigned char *out)
{
    size_t pos = ctx->pos & 63;
    ctx->buf[pos++] = 0x80;
    if (pos > 56) {
        memset(ctx->buf + pos, 0, 64 - pos);
        md5_process_block(ctx, ctx->buf);
        pos = 0;
    }
    memset(ctx->buf + pos, 0, 56 - pos);
    uint64_t bits = (ctx->pos + 0ull) << 3;
    ctx->buf[56] = bits;
    ctx->buf[57] = bits >> 8;
    ctx->buf[58] = bits >> 16;
    ctx->buf[59] = bits >> 24;
    ctx->buf[60] = bits >> 32;
    ctx->buf[61] = bits >> 40;
    ctx->buf[62] = bits >> 48;
    ctx->buf[63] = bits >> 56;
    md5_process_block(ctx, ctx->buf);
    out[0] = ctx->h[0];
    out[1] = ctx->h[0] >> 8;
    out[2] = ctx->h[0] >> 16;
    out[3] = ctx->h[0] >> 24;
    out[4] = ctx->h[1];
    out[5] = ctx->h[1] >> 8;
    out[6] = ctx->h[1] >> 16;
    out[7] = ctx->h[1] >> 24;
    out[8] = ctx->h[2];
    out[9] = ctx->h[2] >> 8;
    out[10] = ctx->h[2] >> 16;
    out[11] = ctx->h[2] >> 24;
    out[12] = ctx->h[3];
    out[13] = ctx->h[3] >> 8;
    out[14] = ctx->h[3] >> 16;
    out[15] = ctx->h[3] >> 24;
}

static char *md5crypt(const char *pass, const char *salt, char *buf)
{
    size_t pwdlen = strlen(pass);

    if (memcmp(salt, "$1$", 3)) return 0;
    salt += 3;

    size_t slen;
    for (slen = 0; slen < 8 && salt[slen] && salt[slen] != '$'; slen++);

    struct md5ctx ctx;
    unsigned char hashbuf[16];

    md5_init_ctx(&ctx);
    md5_add_bytes(&ctx, pass, pwdlen);
    md5_add_bytes(&ctx, salt, slen);
    md5_add_bytes(&ctx, pass, pwdlen);
    md5_finalize(&ctx, hashbuf);

    md5_init_ctx(&ctx);
    md5_add_bytes(&ctx, pass, pwdlen);
    md5_add_bytes(&ctx, salt - 3, slen + 3);
    size_t i;
    for (i = pwdlen; i > sizeof hashbuf; i -= sizeof hashbuf)
        md5_add_bytes(&ctx, hashbuf, sizeof hashbuf);
    md5_add_bytes(&ctx, hashbuf, i);
    for (i = pwdlen; i; i >>= 1)
        if (i & 1)
            md5_add_bytes(&ctx, &(unsigned char){0}, 1);
        else
            md5_add_bytes(&ctx, pass, 1);
    md5_finalize(&ctx, hashbuf);

    for (i = 0; i < 1000; i++) {
        md5_init_ctx(&ctx);
        if (i & 1)
            md5_add_bytes(&ctx, pass, pwdlen);
        else
            md5_add_bytes(&ctx, hashbuf, sizeof hashbuf);
        if (i % 3)
            md5_add_bytes(&ctx, salt, slen);
        if (i % 7)
            md5_add_bytes(&ctx, pass, pwdlen);
        if (i & 1)
            md5_add_bytes(&ctx, hashbuf, sizeof hashbuf);
        else
            md5_add_bytes(&ctx, pass, pwdlen);
        md5_finalize(&ctx, hashbuf);
    }

    memcpy(buf, salt - 3, slen + 3);
    char *p = buf + slen + 3;
    *p++ = '$';

    static const unsigned char perm[] = {
        0,6,12,
        1,7,13,
        2,8,14,
        3,9,15,
        4,10,5
    };
    static const char alphabet[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    unsigned tmp;
    for (i = 0; i < 5; i++) {
        tmp = hashbuf[perm[3*i]] << 16 | hashbuf[perm[3*i+1]] << 8 | hashbuf[perm[3*i+2]];
        *p++ = alphabet[tmp & 63];
        *p++ = alphabet[(tmp >> 6) & 63];
        *p++ = alphabet[(tmp >> 12) & 63];
        *p++ = alphabet[(tmp >> 18) & 63];
    }
    tmp = hashbuf[11];
    *p++ = alphabet[tmp & 63];
    *p++ = alphabet[(tmp >> 6) & 63];
    *p++ = 0;
    return buf;
}

hidden char *__md5crypt(const char *pass, const char *setting, char *buf)
{
    static const char testpass[] = "Xy01@#\x01\x02\x80\x7f\xff\r\n\x81\t !";
    static const char testsetting[] = "$1$abcd0123";
    static const char testhash[] = "$1$abcd0123$9Qcg8DyviekV3tDGMZynJ1";
    char testbuf[64];

    char *res = md5crypt(pass, setting, buf);
    char *p;
    __asm__("" : "=r"(p) : "0"(testpass), "r"(res));
    char *q = md5crypt(p, testsetting, testbuf);
    if (!q || strcmp(q, testhash))
        return 0;
    return res;
}
