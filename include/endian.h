#ifndef __ENDIAN_H
#define __ENDIAN_H

#define __NEED_uint16_t
#define __NEED_uint32_t
#define __NEED_uint64_t
#include <alltypes.h>
#include <features.h>
#include <bswap.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline uint64_t __bswap64(uint64_t __x) {
    return (__bswap32(__x) + 0ull) << 32 | __bswap32(__x >> 32);
}

#define LITTLE_ENDIAN 4321
#define BIG_ENDIAN 1234

#ifdef __LITTLE_ENDIAN
#define BYTE_ORDER LITTLE_ENDIAN
#define be16toh(x) __bswap16(x)
#define be32toh(x) __bswap32(x)
#define be64toh(x) __bswap64(x)
#define htobe16(x) __bswap16(x)
#define htobe32(x) __bswap32(x)
#define htobe64(x) __bswap64(x)
#define le16toh(x) ((uint16_t)((x) * 1))
#define le32toh(x) ((uint32_t)((x) * 1))
#define le64toh(x) ((uint64_t)((x) * 1))
#define htole16(x) ((uint16_t)((x) * 1))
#define htole32(x) ((uint32_t)((x) * 1))
#define htole64(x) ((uint64_t)((x) * 1))
#else
#define BYTE_ORDER BIG_ENDIAN
#define be16toh(x) ((uint16_t)((x) * 1))
#define be32toh(x) ((uint32_t)((x) * 1))
#define be64toh(x) ((uint64_t)((x) * 1))
#define htobe16(x) ((uint16_t)((x) * 1))
#define htobe32(x) ((uint32_t)((x) * 1))
#define htobe64(x) ((uint64_t)((x) * 1))
#define le16toh(x) __bswap16(x)
#define le32toh(x) __bswap32(x)
#define le64toh(x) __bswap64(x)
#define htole16(x) __bswap16(x)
#define htole32(x) __bswap32(x)
#define htole64(x) __bswap64(x)
#endif

#ifdef __cplusplus
}
#endif
#endif
