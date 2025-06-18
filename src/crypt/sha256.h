#ifndef SHA256_H
#define SHA256_H

#include <stddef.h>
#include <stdint.h>
#include <features.h>
struct sha256_ctx {
    uint32_t h[8];
    size_t total;
    unsigned char buf[64];
};

hidden void __sha256_init_ctx(struct sha256_ctx *ctx);
hidden void __sha256_add_bytes(struct sha256_ctx *ctx, const void *data, size_t len);
hidden void __sha256_finalize(struct sha256_ctx *ctx, unsigned char *digest);
hidden void __sha256_buf(const void *, size_t, unsigned char *);
hidden void __hmac_sha256_buf(const void *, size_t, const void *, size_t, unsigned char *);
hidden void __pbkdf2_sha256(const void *, size_t, const void *, size_t, uint64_t, unsigned char *, size_t);
#endif
