#include "crypt.h"
/* originally: yescrypt.h */
/*-
 * Copyright 2009 Colin Percival
 * Copyright 2013-2018 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Colin Percival as part of the Tarsnap
 * online backup system.
 */
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void *base, *aligned;
    size_t base_size, aligned_size;
} yescrypt_region_t;

typedef yescrypt_region_t yescrypt_shared_t;
typedef yescrypt_region_t yescrypt_local_t;

#define YESCRYPT_ROM_TAG1 0x7470797263736579ULL /* "yescrypt" */
#define YESCRYPT_ROM_TAG2 0x687361684d4f522dULL /* "-ROMhash" */

typedef uint32_t yescrypt_flags_t;

#define YESCRYPT_WORM			1
#define YESCRYPT_RW			0x002
#define YESCRYPT_ROUNDS_3		0x000
#define YESCRYPT_ROUNDS_6		0x004
#define YESCRYPT_GATHER_1		0x000
#define YESCRYPT_GATHER_2		0x008
#define YESCRYPT_GATHER_4		0x010
#define YESCRYPT_GATHER_8		0x018
#define YESCRYPT_SIMPLE_1		0x000
#define YESCRYPT_SIMPLE_2		0x020
#define YESCRYPT_SIMPLE_4		0x040
#define YESCRYPT_SIMPLE_8		0x060
#define YESCRYPT_SBOX_6K		0x000
#define YESCRYPT_SBOX_12K		0x080
#define YESCRYPT_SBOX_24K		0x100
#define YESCRYPT_SBOX_48K		0x180
#define YESCRYPT_SBOX_96K		0x200
#define YESCRYPT_SBOX_192K		0x280
#define YESCRYPT_SBOX_384K		0x300
#define YESCRYPT_SBOX_768K		0x380
#define YESCRYPT_SHARED_PREALLOCATED	0x10000

#define YESCRYPT_MODE_MASK		0x003
#define YESCRYPT_RW_FLAVOR_MASK		0x3fc
#define YESCRYPT_INIT_SHARED		0x01000000
#define YESCRYPT_ALLOC_ONLY		0x08000000
#define YESCRYPT_PREHASH		0x10000000

#define YESCRYPT_RW_DEFAULTS \
    (YESCRYPT_RW | \
     YESCRYPT_ROUNDS_6 | YESCRYPT_GATHER_4 | YESCRYPT_SIMPLE_2 | \
     YESCRYPT_SBOX_12K)

#define YESCRYPT_DEFAULTS YESCRYPT_RW_DEFAULTS

#define YESCRYPT_KNOWN_FLAGS \
    (YESCRYPT_MODE_MASK | YESCRYPT_RW_FLAVOR_MASK | \
     YESCRYPT_SHARED_PREALLOCATED | \
     YESCRYPT_INIT_SHARED | YESCRYPT_ALLOC_ONLY | YESCRYPT_PREHASH)

typedef struct {
    yescrypt_flags_t flags;
    uint64_t N;
    uint32_t r, p, t, g;
    uint64_t NROM;
} yescrypt_params_t;

typedef union {
    unsigned char uc[32];
    uint64_t u64[4];
} yescrypt_binary_t;

/* originally: yescrypt-platform.c */
/*-
 * Copyright 2013-2018,2022 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/mman.h>

static void *alloc_region(yescrypt_region_t *region, size_t size)
{
    size_t base_size = size;
    uint8_t *base, *aligned;
    int flags = MAP_ANONYMOUS | MAP_PRIVATE;
    base = (void *)mmap(NULL, size, PROT_READ | PROT_WRITE, flags, -1, 0);
    if (base == MAP_FAILED)
        base = NULL;
    aligned = base;
    region->base = base;
    region->aligned = aligned;
    region->base_size = base ? base_size : 0;
    region->aligned_size = base ? size : 0;
    return aligned;
}

static inline void init_region(yescrypt_region_t *region)
{
    region->base = region->aligned = NULL;
    region->base_size = region->aligned_size = 0;
}

static int free_region(yescrypt_region_t *region)
{
    if (region->base) {
        if (munmap(region->base, region->base_size))
            return -1;
    }
    init_region(region);
    return 0;
}

/* originally: yescrypt-opt.c */
/*-
 * Copyright 2009 Colin Percival
 * Copyright 2012-2025 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Colin Percival as part of the Tarsnap
 * online backup system.
 */

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sha256.h"

#ifdef __GNUC__
#define unlikely(exp) __builtin_expect(exp, 0)
#else
#define unlikely(exp) (exp)
#endif

#undef PREFETCH

typedef union {
    uint32_t w[16];
    uint64_t d[8];
} salsa20_blk_t;

static inline void salsa20_simd_shuffle(const salsa20_blk_t *Bin,
        salsa20_blk_t *Bout)
{
    #define COMBINE(out, in1, in2) \
    Bout->d[out] = Bin->w[in1 * 2] | ((uint64_t)Bin->w[in2 * 2 + 1] << 32);
    COMBINE(0, 0, 2)
    COMBINE(1, 5, 7)
    COMBINE(2, 2, 4)
    COMBINE(3, 7, 1)
    COMBINE(4, 4, 6)
    COMBINE(5, 1, 3)
    COMBINE(6, 6, 0)
    COMBINE(7, 3, 5)
    #undef COMBINE
}

static inline void salsa20_simd_unshuffle(const salsa20_blk_t *Bin,
        salsa20_blk_t *Bout)
{
    #define UNCOMBINE(out, in1, in2) \
    Bout->w[out * 2] = Bin->d[in1]; \
    Bout->w[out * 2 + 1] = Bin->d[in2] >> 32;
    UNCOMBINE(0, 0, 6)
    UNCOMBINE(1, 5, 3)
    UNCOMBINE(2, 2, 0)
    UNCOMBINE(3, 7, 5)
    UNCOMBINE(4, 4, 2)
    UNCOMBINE(5, 1, 7)
    UNCOMBINE(6, 6, 4)
    UNCOMBINE(7, 3, 1)
    #undef UNCOMBINE
}

#define DECL_X \
    salsa20_blk_t X;
#define DECL_Y \
    salsa20_blk_t Y;

#define COPY(out, in) \
    (out).d[0] = (in).d[0]; \
    (out).d[1] = (in).d[1]; \
    (out).d[2] = (in).d[2]; \
    (out).d[3] = (in).d[3]; \
    (out).d[4] = (in).d[4]; \
    (out).d[5] = (in).d[5]; \
    (out).d[6] = (in).d[6]; \
    (out).d[7] = (in).d[7];

#define READ_X(in) COPY(X, in)
#define WRITE_X(out) COPY(out, X)

static inline void salsa20(salsa20_blk_t *restrict B,
        salsa20_blk_t *restrict Bout, uint32_t doublerounds)
{
    salsa20_blk_t X;
    #define x X.w

    salsa20_simd_unshuffle(B, &X);

    do {
        #define R(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
        /* Operate on columns */
        x[ 4] ^= R(x[ 0]+x[12], 7);  x[ 8] ^= R(x[ 4]+x[ 0], 9);
        x[12] ^= R(x[ 8]+x[ 4],13);  x[ 0] ^= R(x[12]+x[ 8],18);

        x[ 9] ^= R(x[ 5]+x[ 1], 7);  x[13] ^= R(x[ 9]+x[ 5], 9);
        x[ 1] ^= R(x[13]+x[ 9],13);  x[ 5] ^= R(x[ 1]+x[13],18);

        x[14] ^= R(x[10]+x[ 6], 7);  x[ 2] ^= R(x[14]+x[10], 9);
        x[ 6] ^= R(x[ 2]+x[14],13);  x[10] ^= R(x[ 6]+x[ 2],18);

        x[ 3] ^= R(x[15]+x[11], 7);  x[ 7] ^= R(x[ 3]+x[15], 9);
        x[11] ^= R(x[ 7]+x[ 3],13);  x[15] ^= R(x[11]+x[ 7],18);

        /* Operate on rows */
        x[ 1] ^= R(x[ 0]+x[ 3], 7);  x[ 2] ^= R(x[ 1]+x[ 0], 9);
        x[ 3] ^= R(x[ 2]+x[ 1],13);  x[ 0] ^= R(x[ 3]+x[ 2],18);

        x[ 6] ^= R(x[ 5]+x[ 4], 7);  x[ 7] ^= R(x[ 6]+x[ 5], 9);
        x[ 4] ^= R(x[ 7]+x[ 6],13);  x[ 5] ^= R(x[ 4]+x[ 7],18);

        x[11] ^= R(x[10]+x[ 9], 7);  x[ 8] ^= R(x[11]+x[10], 9);
        x[ 9] ^= R(x[ 8]+x[11],13);  x[10] ^= R(x[ 9]+x[ 8],18);

        x[12] ^= R(x[15]+x[14], 7);  x[13] ^= R(x[12]+x[15], 9);
        x[14] ^= R(x[13]+x[12],13);  x[15] ^= R(x[14]+x[13],18);
        #undef R
    } while (--doublerounds);
    #undef x

    {
        uint32_t i;
        salsa20_simd_shuffle(&X, Bout);
        for (i = 0; i < 16; i += 4) {
            B->w[i] = Bout->w[i] += B->w[i];
            B->w[i + 1] = Bout->w[i + 1] += B->w[i + 1];
            B->w[i + 2] = Bout->w[i + 2] += B->w[i + 2];
            B->w[i + 3] = Bout->w[i + 3] += B->w[i + 3];
        }
    }
}

#define SALSA20_2(out) \
    salsa20(&X, &out, 1);

#define XOR(out, in1, in2) \
    (out).d[0] = (in1).d[0] ^ (in2).d[0]; \
    (out).d[1] = (in1).d[1] ^ (in2).d[1]; \
    (out).d[2] = (in1).d[2] ^ (in2).d[2]; \
    (out).d[3] = (in1).d[3] ^ (in2).d[3]; \
    (out).d[4] = (in1).d[4] ^ (in2).d[4]; \
    (out).d[5] = (in1).d[5] ^ (in2).d[5]; \
    (out).d[6] = (in1).d[6] ^ (in2).d[6]; \
    (out).d[7] = (in1).d[7] ^ (in2).d[7];

#define XOR_X(in) XOR(X, X, in)
#define XOR_X_2(in1, in2) XOR(X, in1, in2)
#define XOR_X_WRITE_XOR_Y_2(out, in) \
    XOR(Y, out, in) \
    COPY(out, Y) \
    XOR(X, X, Y)

#define SALSA20_8_XOR_MEM(in, out) \
    XOR_X(in); \
    salsa20(&X, &out, 4);

#define INTEGERIFY (uint32_t)X.d[0]

static void blockmix_salsa8(const salsa20_blk_t *restrict Bin,
        salsa20_blk_t *restrict Bout, size_t r)
{
    size_t i;
    DECL_X

    READ_X(Bin[r * 2 - 1])
    for (i = 0; i < r; i++) {
        SALSA20_8_XOR_MEM(Bin[i * 2], Bout[i])
        SALSA20_8_XOR_MEM(Bin[i * 2 + 1], Bout[r + i])
    }
}

static uint32_t blockmix_salsa8_xor(const salsa20_blk_t *restrict Bin1,
        const salsa20_blk_t *restrict Bin2, salsa20_blk_t *restrict Bout,
        size_t r)
{
    size_t i;
    DECL_X

    #ifdef PREFETCH
    PREFETCH(&Bin2[r * 2 - 1], _MM_HINT_T0)
    for (i = 0; i < r - 1; i++) {
        PREFETCH(&Bin2[i * 2], _MM_HINT_T0)
            PREFETCH(&Bin2[i * 2 + 1], _MM_HINT_T0)
    }
    PREFETCH(&Bin2[i * 2], _MM_HINT_T0)
    #endif

    XOR_X_2(Bin1[r * 2 - 1], Bin2[r * 2 - 1])
    for (i = 0; i < r; i++) {
        XOR_X(Bin1[i * 2])
            SALSA20_8_XOR_MEM(Bin2[i * 2], Bout[i])
            XOR_X(Bin1[i * 2 + 1])
            SALSA20_8_XOR_MEM(Bin2[i * 2 + 1], Bout[r + i])
    }

    return INTEGERIFY;
}

/* This is tunable */
#define Swidth 8

/* Not tunable in this implementation, hard-coded in a few places */
#define PWXsimple 2
#define PWXgather 4

/* Derived values.  Not tunable except via Swidth above. */
#define PWXbytes (PWXgather * PWXsimple * 8)
#define Sbytes (3 * (1 << Swidth) * PWXsimple * 8)
#define Smask (((1 << Swidth) - 1) * PWXsimple * 8)
#define Smask2 (((uint64_t)Smask << 32) | Smask)

#define DECL_SMASK2REG /* empty */
#define FORCE_REGALLOC_3 /* empty */
#define MAYBE_MEMORY_BARRIER /* empty */

#define PWXFORM_SIMD(x0, x1) { \
    uint64_t x = x0 & Smask2; \
    uint64_t *p0 = (uint64_t *)(S0 + (uint32_t)x); \
    uint64_t *p1 = (uint64_t *)(S1 + (x >> 32)); \
    x0 = ((x0 >> 32) * (uint32_t)x0 + p0[0]) ^ p1[0]; \
    x1 = ((x1 >> 32) * (uint32_t)x1 + p0[1]) ^ p1[1]; \
}

#define PWXFORM_ROUND \
    PWXFORM_SIMD(X.d[0], X.d[1]) \
PWXFORM_SIMD(X.d[2], X.d[3]) \
PWXFORM_SIMD(X.d[4], X.d[5]) \
PWXFORM_SIMD(X.d[6], X.d[7])

/*
 * This offset helps address the 256-byte write block via the single-byte
 * displacements encodable in x86(-64) instructions.  It is needed because the
 * displacements are signed.  Without it, we'd get 4-byte displacements for
 * half of the writes.  Setting it to 0x80 instead of 0x7c would avoid needing
 * a displacement for one of the writes, but then the LEA instruction would
 * need a 4-byte displacement.
 */
#define PWXFORM_WRITE_OFFSET 0x7c

#define PWXFORM_WRITE \
    WRITE_X(*(salsa20_blk_t *)(Sw - PWXFORM_WRITE_OFFSET)) \
Sw += 64;

#define PWXFORM { \
    uint8_t *Sw = S2 + w + PWXFORM_WRITE_OFFSET; \
    FORCE_REGALLOC_3 \
    MAYBE_MEMORY_BARRIER \
    PWXFORM_ROUND \
    PWXFORM_ROUND PWXFORM_WRITE \
    PWXFORM_ROUND PWXFORM_WRITE \
    PWXFORM_ROUND PWXFORM_WRITE \
    PWXFORM_ROUND PWXFORM_WRITE \
    PWXFORM_ROUND \
    w = (w + 64 * 4) & Smask2; \
    { \
        uint8_t *Stmp = S2; \
        S2 = S1; \
        S1 = S0; \
        S0 = Stmp; \
    } \
}

typedef struct {
    uint8_t *S0, *S1, *S2;
    size_t w;
} pwxform_ctx_t;

#define Salloc (Sbytes + ((sizeof(pwxform_ctx_t) + 63) & ~63U))

static void blockmix(const salsa20_blk_t *restrict Bin,
        salsa20_blk_t *restrict Bout, size_t r, pwxform_ctx_t *restrict ctx)
{
    uint8_t *S0 = ctx->S0, *S1 = ctx->S1, *S2 = ctx->S2;
    size_t w = ctx->w;
    size_t i;
    DECL_X

        /* Convert count of 128-byte blocks to max index of 64-byte block */
        r = r * 2 - 1;

    READ_X(Bin[r])

    DECL_SMASK2REG

    i = 0;
    do {
        XOR_X(Bin[i])
        PWXFORM
        if (unlikely(i >= r))
            break;
        WRITE_X(Bout[i])
        i++;
    } while (1);

    ctx->S0 = S0; ctx->S1 = S1; ctx->S2 = S2;
    ctx->w = w;

    SALSA20_2(Bout[i])
}

static uint32_t blockmix_xor(const salsa20_blk_t *Bin1,
        const salsa20_blk_t *restrict Bin2, salsa20_blk_t *Bout,
        size_t r, int Bin2_in_ROM, pwxform_ctx_t *restrict ctx)
{
    uint8_t *S0 = ctx->S0, *S1 = ctx->S1, *S2 = ctx->S2;
    size_t w = ctx->w;
    size_t i;
    DECL_X

    /* Convert count of 128-byte blocks to max index of 64-byte block */
    r = r * 2 - 1;

    #ifdef PREFETCH
    if (Bin2_in_ROM) {
        PREFETCH(&Bin2[r], _MM_HINT_NTA)
        for (i = 0; i < r; i++) {
            PREFETCH(&Bin2[i], _MM_HINT_NTA)
        }
    } else {
        PREFETCH(&Bin2[r], _MM_HINT_T0)
        for (i = 0; i < r; i++) {
            PREFETCH(&Bin2[i], _MM_HINT_T0)
        }
    }
    #else
    (void)Bin2_in_ROM; /* unused */
    #endif

    XOR_X_2(Bin1[r], Bin2[r])

    DECL_SMASK2REG

    i = 0;
    r--;
    do {
        XOR_X(Bin1[i])
        XOR_X(Bin2[i])
        PWXFORM
        WRITE_X(Bout[i])

        XOR_X(Bin1[i + 1])
        XOR_X(Bin2[i + 1])
        PWXFORM

        if (unlikely(i >= r))
            break;

        WRITE_X(Bout[i + 1])

            i += 2;
    } while (1);
    i++;

    ctx->S0 = S0; ctx->S1 = S1; ctx->S2 = S2;
    ctx->w = w;

    SALSA20_2(Bout[i])

    return INTEGERIFY;
}

static uint32_t blockmix_xor_save(salsa20_blk_t *restrict Bin1out,
        salsa20_blk_t *restrict Bin2,
        size_t r, pwxform_ctx_t *restrict ctx)
{
    uint8_t *S0 = ctx->S0, *S1 = ctx->S1, *S2 = ctx->S2;
    size_t w = ctx->w;
    size_t i;
    DECL_X
    DECL_Y

    /* Convert count of 128-byte blocks to max index of 64-byte block */
    r = r * 2 - 1;

    #ifdef PREFETCH
    PREFETCH(&Bin2[r], _MM_HINT_T0)
    for (i = 0; i < r; i++) {
        PREFETCH(&Bin2[i], _MM_HINT_T0)
    }
    #endif

    XOR_X_2(Bin1out[r], Bin2[r])

    DECL_SMASK2REG

    i = 0;
    r--;
    do {
        XOR_X_WRITE_XOR_Y_2(Bin2[i], Bin1out[i])
        PWXFORM
        WRITE_X(Bin1out[i])

        XOR_X_WRITE_XOR_Y_2(Bin2[i + 1], Bin1out[i + 1])
        PWXFORM

        if (unlikely(i >= r))
            break;

        WRITE_X(Bin1out[i + 1])

        i += 2;
    } while (1);
    i++;

    ctx->S0 = S0; ctx->S1 = S1; ctx->S2 = S2;
    ctx->w = w;

    SALSA20_2(Bin1out[i])

    return INTEGERIFY;
}

static inline uint32_t integerify(const salsa20_blk_t *B, size_t r)
{
    /*
     * Our 64-bit words are in host byte order, which is why we don't just read
     * w[0] here (would be wrong on big-endian).  Also, our 32-bit words are
     * SIMD-shuffled (so the next 32 bits would be part of d[6]), but currently
     * this does not matter as we only care about the least significant 32 bits.
     */
    return (uint32_t)B[2 * r - 1].d[0];
}

static uint32_t le32dec(const void *vd)
{
    const unsigned char *d = vd;
    return d[0] | d[1] << 8 | d[2] << 16 | (d[3] + 0ul) << 24;
}

static void le32enc(void *vd, uint32_t x)
{
    unsigned char *d = vd;
    d[0] = x;
    d[1] = x >> 8;
    d[2] = x >> 16;
    d[3] = x >> 24;
}

static void smix1(uint8_t *B, size_t r, uint32_t N, yescrypt_flags_t flags,
        salsa20_blk_t *V, uint32_t NROM, const salsa20_blk_t *VROM,
        salsa20_blk_t *XY, pwxform_ctx_t *ctx)
{
    size_t s = 2 * r;
    salsa20_blk_t *X = V, *Y = &V[s];
    uint32_t i, j;

    for (i = 0; i < 2 * r; i++) {
        const salsa20_blk_t *src = (salsa20_blk_t *)&B[i * 64];
        salsa20_blk_t *tmp = Y;
        salsa20_blk_t *dst = &X[i];
        size_t k;
        for (k = 0; k < 16; k++)
            tmp->w[k] = le32dec(&src->w[k]);
        salsa20_simd_shuffle(tmp, dst);
    }

    if (VROM) {
        uint32_t n;
        const salsa20_blk_t *V_j;

        V_j = &VROM[(NROM - 1) * s];
        j = blockmix_xor(X, V_j, Y, r, 1, ctx) & (NROM - 1);
        V_j = &VROM[j * s];
        X = Y + s;
        j = blockmix_xor(Y, V_j, X, r, 1, ctx);

        for (n = 2; n < N; n <<= 1) {
            uint32_t m = (n < N / 2) ? n : (N - 1 - n);
            for (i = 1; i < m; i += 2) {
                j &= n - 1;
                j += i - 1;
                V_j = &V[j * s];
                Y = X + s;
                j = blockmix_xor(X, V_j, Y, r, 0, ctx) & (NROM - 1);
                V_j = &VROM[j * s];
                X = Y + s;
                j = blockmix_xor(Y, V_j, X, r, 1, ctx);
            }
        }
        n >>= 1;

        j &= n - 1;
        j += N - 2 - n;
        V_j = &V[j * s];
        Y = X + s;
        j = blockmix_xor(X, V_j, Y, r, 0, ctx) & (NROM - 1);
        V_j = &VROM[j * s];
        blockmix_xor(Y, V_j, XY, r, 1, ctx);
    } else if (flags & YESCRYPT_RW) {
        uint32_t n;
        salsa20_blk_t *V_j;

        blockmix(X, Y, r, ctx);
        X = Y + s;
        blockmix(Y, X, r, ctx);
        j = integerify(X, r);

        for (n = 2; n < N; n <<= 1) {
            uint32_t m = (n < N / 2) ? n : (N - 1 - n);
            for (i = 1; i < m; i += 2) {
                Y = X + s;
                j &= n - 1;
                j += i - 1;
                V_j = &V[j * s];
                j = blockmix_xor(X, V_j, Y, r, 0, ctx);
                j &= n - 1;
                j += i;
                V_j = &V[j * s];
                X = Y + s;
                j = blockmix_xor(Y, V_j, X, r, 0, ctx);
            }
        }
        n >>= 1;

        j &= n - 1;
        j += N - 2 - n;
        V_j = &V[j * s];
        Y = X + s;
        j = blockmix_xor(X, V_j, Y, r, 0, ctx);
        j &= n - 1;
        j += N - 1 - n;
        V_j = &V[j * s];
        blockmix_xor(Y, V_j, XY, r, 0, ctx);
    } else {
        N -= 2;
        do {
            blockmix_salsa8(X, Y, r);
            X = Y + s;
            blockmix_salsa8(Y, X, r);
            Y = X + s;
        } while ((N -= 2));

        blockmix_salsa8(X, Y, r);
        blockmix_salsa8(Y, XY, r);
    }

    for (i = 0; i < 2 * r; i++) {
        const salsa20_blk_t *src = &XY[i];
        salsa20_blk_t *tmp = &XY[s];
        salsa20_blk_t *dst = (salsa20_blk_t *)&B[i * 64];
        size_t k;
        for (k = 0; k < 16; k++)
            le32enc(&tmp->w[k], src->w[k]);
        salsa20_simd_unshuffle(tmp, dst);
    }
}

static void smix2(uint8_t *B, size_t r, uint32_t N, uint64_t Nloop,
        yescrypt_flags_t flags, salsa20_blk_t *V, uint32_t NROM,
        const salsa20_blk_t *VROM, salsa20_blk_t *XY, pwxform_ctx_t *ctx)
{
    size_t s = 2 * r;
    salsa20_blk_t *X = XY, *Y = &XY[s];
    uint32_t i, j;

    if (Nloop == 0)
        return;

    for (i = 0; i < 2 * r; i++) {
        const salsa20_blk_t *src = (salsa20_blk_t *)&B[i * 64];
        salsa20_blk_t *tmp = Y;
        salsa20_blk_t *dst = &X[i];
        size_t k;
        for (k = 0; k < 16; k++)
            tmp->w[k] = le32dec(&src->w[k]);
        salsa20_simd_shuffle(tmp, dst);
    }

    j = integerify(X, r) & (N - 1);

    /*
     * Normally, VROM implies YESCRYPT_RW, but we check for these separately
     * because our SMix resets YESCRYPT_RW for the smix2() calls operating on the
     * entire V when p > 1.
     */
    if (VROM && (flags & YESCRYPT_RW)) {
        do {
            salsa20_blk_t *V_j = &V[j * s];
            const salsa20_blk_t *VROM_j;
            j = blockmix_xor_save(X, V_j, r, ctx) & (NROM - 1);
            VROM_j = &VROM[j * s];
            j = blockmix_xor(X, VROM_j, X, r, 1, ctx) & (N - 1);
        } while (Nloop -= 2);
    } else if (VROM) {
        do {
            const salsa20_blk_t *V_j = &V[j * s];
            j = blockmix_xor(X, V_j, X, r, 0, ctx) & (NROM - 1);
            V_j = &VROM[j * s];
            j = blockmix_xor(X, V_j, X, r, 1, ctx) & (N - 1);
        } while (Nloop -= 2);
    } else if (flags & YESCRYPT_RW) {
        do {
            salsa20_blk_t *V_j = &V[j * s];
            j = blockmix_xor_save(X, V_j, r, ctx) & (N - 1);
            V_j = &V[j * s];
            j = blockmix_xor_save(X, V_j, r, ctx) & (N - 1);
        } while (Nloop -= 2);
    } else if (ctx) {
        do {
            const salsa20_blk_t *V_j = &V[j * s];
            j = blockmix_xor(X, V_j, X, r, 0, ctx) & (N - 1);
            V_j = &V[j * s];
            j = blockmix_xor(X, V_j, X, r, 0, ctx) & (N - 1);
        } while (Nloop -= 2);
    } else {
        do {
            const salsa20_blk_t *V_j = &V[j * s];
            j = blockmix_salsa8_xor(X, V_j, Y, r) & (N - 1);
            V_j = &V[j * s];
            j = blockmix_salsa8_xor(Y, V_j, X, r) & (N - 1);
        } while (Nloop -= 2);
    }

    for (i = 0; i < 2 * r; i++) {
        const salsa20_blk_t *src = &X[i];
        salsa20_blk_t *tmp = Y;
        salsa20_blk_t *dst = (salsa20_blk_t *)&B[i * 64];
        size_t k;
        for (k = 0; k < 16; k++)
            le32enc(&tmp->w[k], src->w[k]);
        salsa20_simd_unshuffle(tmp, dst);
    }
}

static uint64_t p2floor(uint64_t x)
{
    uint64_t y;
    while ((y = x & (x - 1)))
        x = y;
    return x;
}

static void smix(uint8_t *B, size_t r, uint32_t N, uint32_t p, uint32_t t,
        yescrypt_flags_t flags,
        salsa20_blk_t *V, uint32_t NROM, const salsa20_blk_t *VROM,
        salsa20_blk_t *XY, uint8_t *S, uint8_t *passwd)
{
    size_t s = 2 * r;
    uint32_t Nchunk;
    uint64_t Nloop_all, Nloop_rw;
    uint32_t i;

    Nchunk = N / p;
    Nloop_all = Nchunk;
    if (flags & YESCRYPT_RW) {
        if (t <= 1) {
            if (t)
                Nloop_all *= 2; /* 2/3 */
            Nloop_all = (Nloop_all + 2) / 3; /* 1/3, round up */
        } else {
            Nloop_all *= t - 1;
        }
    } else if (t) {
        if (t == 1)
            Nloop_all += (Nloop_all + 1) / 2; /* 1.5, round up */
        Nloop_all *= t;
    }

    Nloop_rw = 0;
    if (flags & YESCRYPT_INIT_SHARED)
        Nloop_rw = Nloop_all;
    else if (flags & YESCRYPT_RW)
        Nloop_rw = Nloop_all / p;

    Nchunk &= ~(uint32_t)1; /* round down to even */
    Nloop_all++; Nloop_all &= ~(uint64_t)1; /* round up to even */
    Nloop_rw++; Nloop_rw &= ~(uint64_t)1; /* round up to even */

    for (i = 0; i < p; i++) {
        uint32_t Vchunk = i * Nchunk;
        uint32_t Np = (i < p - 1) ? Nchunk : (N - Vchunk);
        uint8_t *Bp = &B[128 * r * i];
        salsa20_blk_t *Vp = &V[Vchunk * s];
        salsa20_blk_t *XYp = XY;
        pwxform_ctx_t *ctx_i = NULL;
        if (flags & YESCRYPT_RW) {
            uint8_t *Si = S + i * Salloc;
            smix1(Bp, 1, Sbytes / 128, 0 /* no flags */,
                    (salsa20_blk_t *)Si, 0, NULL, XYp, NULL);
            ctx_i = (pwxform_ctx_t *)(Si + Sbytes);
            ctx_i->S2 = Si;
            ctx_i->S1 = Si + Sbytes / 3;
            ctx_i->S0 = Si + Sbytes / 3 * 2;
            ctx_i->w = 0;
            if (i == 0)
                __hmac_sha256_buf(Bp + (128 * r - 64), 64,
                        passwd, 32, passwd);
        }
        smix1(Bp, r, Np, flags, Vp, NROM, VROM, XYp, ctx_i);
        smix2(Bp, r, p2floor(Np), Nloop_rw, flags, Vp,
                NROM, VROM, XYp, ctx_i);
    }

    if (Nloop_all > Nloop_rw) {
        for (i = 0; i < p; i++) {
            uint8_t *Bp = &B[128 * r * i];
            salsa20_blk_t *XYp = XY;
            pwxform_ctx_t *ctx_i = NULL;
            if (flags & YESCRYPT_RW) {
                uint8_t *Si = S + i * Salloc;
                ctx_i = (pwxform_ctx_t *)(Si + Sbytes);
            }
            smix2(Bp, r, N, Nloop_all - Nloop_rw,
                    flags & ~YESCRYPT_RW, V, NROM, VROM, XYp, ctx_i);
        }
    }
}

static int yescrypt_kdf_body(const yescrypt_shared_t *shared,
        yescrypt_local_t *local,
        const uint8_t *passwd, size_t passwdlen,
        const uint8_t *salt, size_t saltlen,
        yescrypt_flags_t flags, uint64_t N, uint32_t r, uint32_t p, uint32_t t,
        uint64_t NROM,
        uint8_t *buf, size_t buflen)
{
    yescrypt_region_t tmp;
    const salsa20_blk_t *VROM;
    size_t B_size, V_size, XY_size, need;
    uint8_t *B, *S;
    salsa20_blk_t *V, *XY;
    uint8_t sha256[32];
    uint8_t dk[sizeof(sha256)], *dkp = buf;

    /* Sanity-check parameters */
    switch (flags & YESCRYPT_MODE_MASK) {
        case 0: /* classic scrypt - can't have anything non-standard */
            if (flags || t || NROM)
                goto out_EINVAL;
            break;
        case YESCRYPT_WORM:
            if (flags != YESCRYPT_WORM || NROM)
                goto out_EINVAL;
            break;
        case YESCRYPT_RW:
            if (flags != (flags & YESCRYPT_KNOWN_FLAGS))
                goto out_EINVAL;
            #if PWXsimple == 2 && PWXgather == 4 && Sbytes == 12288
            if ((flags & YESCRYPT_RW_FLAVOR_MASK) ==
                    (YESCRYPT_ROUNDS_6 | YESCRYPT_GATHER_4 |
                     YESCRYPT_SIMPLE_2 | YESCRYPT_SBOX_12K))
                break;
            #else
            #error "Unsupported pwxform settings"
            #endif
            /* FALLTHRU */
        default:
            goto out_EINVAL;
    }
    #if SIZE_MAX > UINT32_MAX
    if (buflen > (((uint64_t)1 << 32) - 1) * 32)
        goto out_EINVAL;
    #endif
    if ((uint64_t)r * (uint64_t)p >= 1 << 30)
        goto out_EINVAL;
    if (N > UINT32_MAX)
        goto out_EINVAL;
    if ((N & (N - 1)) != 0 || N <= 3 || r < 1 || p < 1)
        goto out_EINVAL;
    if (r > SIZE_MAX / 256 / p ||
            N > SIZE_MAX / 128 / r)
        goto out_EINVAL;
    if (flags & YESCRYPT_RW) {
        if (N / p <= 3 || p > SIZE_MAX / Salloc)
            goto out_EINVAL;
    }

    VROM = NULL;
    if (shared) {
        uint64_t expected_size = (size_t)128 * r * NROM;
        if ((NROM & (NROM - 1)) != 0 ||
                NROM <= 1 || NROM > UINT32_MAX ||
                shared->aligned_size < expected_size)
            goto out_EINVAL;
        if (!(flags & YESCRYPT_INIT_SHARED)) {
            uint64_t *tag = (uint64_t *)
                ((uint8_t *)shared->aligned + expected_size - 48);
            if (tag[0] != YESCRYPT_ROM_TAG1 || tag[1] != YESCRYPT_ROM_TAG2)
                goto out_EINVAL;
        }
        VROM = shared->aligned;
    } else {
        if (NROM)
            goto out_EINVAL;
    }

    /* Allocate memory */
    V = NULL;
    V_size = (size_t)128 * r * N;
    need = V_size;
    if (flags & YESCRYPT_INIT_SHARED) {
        if (local->aligned_size < need) {
            if (local->base || local->aligned ||
                    local->base_size || local->aligned_size)
                goto out_EINVAL;
            if (!alloc_region(local, need))
                return -1;
        }
        if (flags & YESCRYPT_ALLOC_ONLY)
            return -2; /* expected "failure" */
        V = (salsa20_blk_t *)local->aligned;
        need = 0;
    }
    B_size = (size_t)128 * r * p;
    need += B_size;
    if (need < B_size)
        goto out_EINVAL;
    XY_size = (size_t)256 * r;
    need += XY_size;
    if (need < XY_size)
        goto out_EINVAL;
    if (flags & YESCRYPT_RW) {
        size_t S_size = (size_t)Salloc * p;
        need += S_size;
        if (need < S_size)
            goto out_EINVAL;
    }
    if (flags & YESCRYPT_INIT_SHARED) {
        if (!alloc_region(&tmp, need))
            return -1;
        B = (uint8_t *)tmp.aligned;
        XY = (salsa20_blk_t *)((uint8_t *)B + B_size);
    } else {
        init_region(&tmp);
        if (local->aligned_size < need) {
            if (free_region(local))
                return -1;
            if (!alloc_region(local, need))
                return -1;
        }
        if (flags & YESCRYPT_ALLOC_ONLY)
            return -3; /* expected "failure" */
        B = (uint8_t *)local->aligned;
        V = (salsa20_blk_t *)((uint8_t *)B + B_size);
        XY = (salsa20_blk_t *)((uint8_t *)V + V_size);
    }
    S = NULL;
    if (flags & YESCRYPT_RW)
        S = (uint8_t *)XY + XY_size;

    if (flags) {
        __hmac_sha256_buf("yescrypt-prehash",
                (flags & YESCRYPT_PREHASH) ? 16 : 8,
                passwd, passwdlen, sha256);
        passwd = sha256;
        passwdlen = sizeof(sha256);
    }

    __pbkdf2_sha256(passwd, passwdlen, salt, saltlen, 1, B, B_size);

    if (flags)
        memcpy(sha256, B, sizeof(sha256));

    if (p == 1 || (flags & YESCRYPT_RW)) {
        smix(B, r, N, p, t, flags, V, NROM, VROM, XY, S, sha256);
    } else {
        uint32_t i;
        for (i = 0; i < p; i++) {
            smix(&B[(size_t)128 * r * i], r, N, 1, t, flags, V,
                    NROM, VROM, XY, NULL, NULL);
        }
    }

    dkp = buf;
    if (flags && buflen < sizeof(dk)) {
        __pbkdf2_sha256(passwd, passwdlen, B, B_size, 1, dk, sizeof(dk));
        dkp = dk;
    }

    __pbkdf2_sha256(passwd, passwdlen, B, B_size, 1, buf, buflen);

    /*
     * Except when computing classic scrypt, allow all computation so far
     * to be performed on the client.  The final steps below match those of
     * SCRAM (RFC 5802), so that an extension of SCRAM (with the steps so
     * far in place of SCRAM's use of PBKDF2 and with SHA-256 in place of
     * SCRAM's use of SHA-1) would be usable with yescrypt hashes.
     */
    if (flags && !(flags & YESCRYPT_PREHASH)) {
        /* Compute ClientKey */
        __hmac_sha256_buf(dkp, sizeof(dk), "Client Key", 10, sha256);
        /* Compute StoredKey */
        {
            size_t clen = buflen;
            if (clen > sizeof(dk))
                clen = sizeof(dk);
            __sha256_buf(sha256, sizeof(sha256), dk);
            memcpy(buf, dk, clen);
        }
    }

    if (free_region(&tmp)) {
        return -1;
    }

    /* Success! */
    return 0;

out_EINVAL:
    errno = EINVAL;
    return -1;
}

static int yescrypt_kdf(const yescrypt_shared_t *shared, yescrypt_local_t *local,
        const uint8_t *passwd, size_t passwdlen,
        const uint8_t *salt, size_t saltlen,
        const yescrypt_params_t *params,
        uint8_t *buf, size_t buflen)
{
    yescrypt_flags_t flags = params->flags;
    uint64_t N = params->N;
    uint32_t r = params->r;
    uint32_t p = params->p;
    uint32_t t = params->t;
    uint32_t g = params->g;
    uint64_t NROM = params->NROM;
    uint8_t dk[32];
    int retval;

    /* Support for hash upgrades has been temporarily removed */
    if (g) {
        errno = EINVAL;
        return -1;
    }

    if ((flags & (YESCRYPT_RW | YESCRYPT_INIT_SHARED)) == YESCRYPT_RW &&
            p >= 1 && N / p >= 0x100 && N / p * r >= 0x20000) {
        if (yescrypt_kdf_body(shared, local,
                    passwd, passwdlen, salt, saltlen,
                    flags | YESCRYPT_ALLOC_ONLY, N, r, p, t, NROM,
                    buf, buflen) != -3) {
            errno = EINVAL;
            return -1;
        }
        if ((retval = yescrypt_kdf_body(shared, local,
                        passwd, passwdlen, salt, saltlen,
                        flags | YESCRYPT_PREHASH, N >> 6, r, p, 0, NROM,
                        dk, sizeof(dk))))
            return retval;
        passwd = dk;
        passwdlen = sizeof(dk);
    }

    retval = yescrypt_kdf_body(shared, local,
            passwd, passwdlen, salt, saltlen,
            flags, N, r, p, t, NROM, buf, buflen);
    return retval;
}

static int yescrypt_init_local(yescrypt_local_t *local)
{
    init_region(local);
    return 0;
}

static int yescrypt_free_local(yescrypt_local_t *local)
{
    return free_region(local);
}

/* originally: yescrypt-common.c */
/*-
 * Copyright 2013-2018 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdint.h>
#include <string.h>

#include "sha256.h"

#define BYTES2CHARS(bytes) ((((bytes) * 8) + 5) / 6)

#define HASH_SIZE sizeof(yescrypt_binary_t) /* bytes */
#define HASH_LEN BYTES2CHARS(HASH_SIZE) /* base-64 chars */

/*
 * "$y$", up to 8 params of up to 6 chars each, '$', salt
 * Alternatively, but that's smaller:
 * "$7$", 3 params encoded as 1+5+5 chars, salt
 */
#define PREFIX_LEN (3 + 8 * 6 + 1 + BYTES2CHARS(32))

static const char * const itoa64 =
"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static const uint8_t atoi64_partial[77] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    64, 64, 64, 64, 64, 64, 64,
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
    64, 64, 64, 64, 64, 64,
    38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
};

static inline uint32_t atoi64(uint8_t src)
{
    if (src >= '.' && src <= 'z')
        return atoi64_partial[src - '.'];

    return 64;
}

static const char *decode64_uint32(uint32_t *dst,
        const char *src, uint32_t min)
{
    uint32_t start = 0, end = 47, chars = 1, bits = 0;
    uint32_t c;

    c = atoi64(*src++);
    if (c > 63)
        goto fail;

    *dst = min;
    while (c > end) {
        *dst += (end + 1 - start) << bits;
        start = end + 1;
        end = start + (62 - end) / 2;
        chars++;
        bits += 6;
    }

    *dst += (c - start) << bits;

    while (--chars) {
        c = atoi64(*src++);
        if (c > 63)
            goto fail;
        bits -= 6;
        *dst += c << bits;
    }

    return src;

fail:
    *dst = 0;
    return NULL;
}

static char *encode64_uint32_fixed(char *dst, size_t dstlen,
        uint32_t src, uint32_t srcbits)
{
    uint32_t bits;

    for (bits = 0; bits < srcbits; bits += 6) {
        if (dstlen < 2)
            return NULL;
        *dst++ = itoa64[src & 0x3f];
        dstlen--;
        src >>= 6;
    }

    if (src || dstlen < 1)
        return NULL;

    *dst = 0; /* NUL terminate just in case */

    return dst;
}

static char *encode64(char *dst, size_t dstlen,
        const uint8_t *src, size_t srclen)
{
    size_t i;

    for (i = 0; i < srclen; ) {
        char *dnext;
        uint32_t value = 0, bits = 0;
        do {
            value |= (uint32_t)src[i++] << bits;
            bits += 8;
        } while (bits < 24 && i < srclen);
        dnext = encode64_uint32_fixed(dst, dstlen, value, bits);
        if (!dnext)
            return NULL;
        dstlen -= dnext - dst;
        dst = dnext;
    }

    if (dstlen < 1)
        return NULL;

    *dst = 0; /* NUL terminate just in case */

    return dst;
}

static const char *decode64_uint32_fixed(uint32_t *dst, uint32_t dstbits,
        const char *src)
{
    uint32_t bits;

    *dst = 0;
    for (bits = 0; bits < dstbits; bits += 6) {
        uint32_t c = atoi64(*src++);
        if (c > 63) {
            *dst = 0;
            return NULL;
        }
        *dst |= c << bits;
    }

    return src;
}

static const char *decode64(uint8_t *dst, size_t *dstlen,
        const char *src, size_t srclen)
{
    size_t dstpos = 0;

    while (dstpos <= *dstlen && srclen) {
        uint32_t value = 0, bits = 0;
        while (srclen--) {
            uint32_t c = atoi64(*src);
            if (c > 63) {
                srclen = 0;
                break;
            }
            src++;
            value |= c << bits;
            bits += 6;
            if (bits >= 24)
                break;
        }
        if (!bits)
            break;
        if (bits < 12) /* must have at least one full byte */
            goto fail;
        while (dstpos++ < *dstlen) {
            *dst++ = value;
            value >>= 8;
            bits -= 8;
            if (bits < 8) { /* 2 or 4 */
                if (value) /* must be 0 */
                    goto fail;
                bits = 0;
                break;
            }
        }
        if (bits)
            goto fail;
    }

    if (!srclen && dstpos <= *dstlen) {
        *dstlen = dstpos;
        return src;
    }

fail:
    *dstlen = 0;
    return NULL;
}

typedef enum { ENC = 1, DEC = -1 } encrypt_dir_t;

static void memxor(unsigned char *dst, unsigned char *src, size_t size)
{
    while (size--)
        *dst++ ^= *src++;
}

static void encrypt(unsigned char *data, size_t datalen,
        const yescrypt_binary_t *key, encrypt_dir_t dir)
{
    struct sha256_ctx ctx;
    unsigned char f[32 + 4];
    size_t halflen, which;
    unsigned char mask, round, target;

    if (!datalen)
        return;
    if (datalen > 64)
        datalen = 64;

    halflen = datalen >> 1;

    which = 0; /* offset to half we are working on (0 or halflen) */
    mask = 0x0f; /* current half's extra nibble mask if datalen is odd */

    round = 0;
    target = 5; /* 6 rounds due to Jacques Patarin's CRYPTO 2004 paper */

    if (dir == DEC) {
        which = halflen; /* even round count, so swap the halves */
        mask ^= 0xff;

        round = target;
        target = 0;
    }

    f[32] = 0;
    f[33] = sizeof(*key);
    f[34] = datalen;

    do {
        __sha256_init_ctx(&ctx);
        f[35] = round;
        __sha256_add_bytes(&ctx, &f[32], 4);
        __sha256_add_bytes(&ctx, key, sizeof(*key));
        __sha256_add_bytes(&ctx, &data[which], halflen);
        if (datalen & 1) {
            f[0] = data[datalen - 1] & mask;
            __sha256_add_bytes(&ctx, f, 1);
        }
        __sha256_finalize(&ctx, f);
        which ^= halflen;
        memxor(&data[which], f, halflen);
        if (datalen & 1) {
            mask ^= 0xff;
            data[datalen - 1] ^= f[halflen] & mask;
        }
        if (round == target)
            break;
        round += dir;
    } while (1);
}

static char *yescrypt_r(const yescrypt_shared_t *shared, yescrypt_local_t *local,
        const char *passwd, size_t passwdlen,
        const char *setting,
        const yescrypt_binary_t *key,
        char *buf, size_t buflen)
{
    unsigned char saltbin[64], hashbin[32];
    const uint8_t *salt;
    const char *src, *saltstr;
    char *dst;
    size_t need, prefixlen, saltstrlen, saltlen;
    yescrypt_params_t params = { .p = 1 };

    if (setting[0] != '$' ||
            (setting[1] != '7' && setting[1] != 'y') ||
            setting[2] != '$')
        return NULL;
    src = setting + 3;

    if (setting[1] == '7') {
        uint32_t N_log2 = atoi64(*src++);
        if (N_log2 < 1 || N_log2 > 63)
            return NULL;
        params.N = (uint64_t)1 << N_log2;

        src = decode64_uint32_fixed(&params.r, 30, src);
        if (!src)
            return NULL;

        src = decode64_uint32_fixed(&params.p, 30, src);
        if (!src)
            return NULL;

        if (key)
            return NULL;
    } else {
        uint32_t flavor, N_log2;

        src = decode64_uint32(&flavor, src, 0);
        if (!src)
            return NULL;

        if (flavor < YESCRYPT_RW) {
            params.flags = flavor;
        } else if (flavor <= YESCRYPT_RW + (YESCRYPT_RW_FLAVOR_MASK >> 2)) {
            params.flags = YESCRYPT_RW + ((flavor - YESCRYPT_RW) << 2);
        } else {
            return NULL;
        }

        src = decode64_uint32(&N_log2, src, 1);
        if (!src || N_log2 > 63)
            return NULL;
        params.N = (uint64_t)1 << N_log2;

        src = decode64_uint32(&params.r, src, 1);
        if (!src)
            return NULL;

        if (*src != '$') {
            uint32_t have;

            src = decode64_uint32(&have, src, 1);
            if (!src)
                return NULL;

            if (have & 1) {
                src = decode64_uint32(&params.p, src, 2);
                if (!src)
                    return NULL;
            }

            if (have & 2) {
                src = decode64_uint32(&params.t, src, 1);
                if (!src)
                    return NULL;
            }

            if (have & 4) {
                src = decode64_uint32(&params.g, src, 1);
                if (!src)
                    return NULL;
            }

            if (have & 8) {
                uint32_t NROM_log2;
                src = decode64_uint32(&NROM_log2, src, 1);
                if (!src || NROM_log2 > 63)
                    return NULL;
                params.NROM = (uint64_t)1 << NROM_log2;
            }
        }

        if (*src++ != '$')
            return NULL;
    }

    prefixlen = src - setting;

    saltstr = src;
    src = strrchr(saltstr, '$');
    if (src)
        saltstrlen = src - saltstr;
    else
        saltstrlen = strlen(saltstr);

    if (setting[1] == '7') {
        salt = (void *)saltstr;
        saltlen = saltstrlen;
    } else {
        const char *saltend;

        saltlen = sizeof(saltbin);
        saltend = decode64(saltbin, &saltlen, saltstr, saltstrlen);

        if (!saltend || (size_t)(saltend - saltstr) != saltstrlen)
            goto fail;

        salt = saltbin;

        if (key)
            encrypt(saltbin, saltlen, key, ENC);
    }

    need = prefixlen + saltstrlen + 1 + HASH_LEN + 1;
    if (need > buflen || need < saltstrlen)
        goto fail;

    if (yescrypt_kdf(shared, local, (void *)passwd, passwdlen, salt, saltlen,
                &params, hashbin, sizeof(hashbin)))
        goto fail;

    if (key) {
        encrypt(hashbin, sizeof(hashbin), key, ENC);
    }

    dst = buf;
    memcpy(dst, setting, prefixlen + saltstrlen);
    dst += prefixlen + saltstrlen;
    *dst++ = '$';

    dst = encode64(dst, buflen - (dst - buf), hashbin, sizeof(hashbin));
    if (!dst || dst >= buf + buflen)
        return NULL;

    *dst = 0; /* NUL termination */

    return buf;

fail:
    return NULL;
}

hidden char *__yescrypt(const char *pass, const char *setting, char *buf)
{
    yescrypt_local_t local;
    char *retval;
    static const char testpass[] = "pleaseletmein";
    static const char testsetting[] = "$y$j//$LdJMENpBABJJ3hIHjB1B$U8a2MaK.yesqWySK8Owk6PWeWmp/XuagMbpP45q1/q1";
    char testbuf[PREFIX_LEN + 1 + HASH_LEN + 1];

    if (yescrypt_init_local(&local))
        return NULL;
    retval = yescrypt_r(NULL, &local, pass, strlen(pass), setting, NULL, buf, CRYPT_LEN);
    char *p;
    __asm__("" : "=r"(p) : "0"(testpass), "r"(retval));
    memset(local.base, 0, local.base_size);
    char *q = yescrypt_r(NULL, &local, p, sizeof testpass - 1, testsetting, 0, testbuf, sizeof testbuf);
    if (yescrypt_free_local(&local))
        return NULL;
    if (q != testbuf || memcmp(testbuf, testsetting, sizeof testsetting))
        retval = 0;
    return retval;
}
