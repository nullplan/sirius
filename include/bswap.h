#ifndef __BSWAP_H
#define __BSWAP_H

#ifdef __cplusplus
extern "C" {
#endif

static inline uint16_t __bswap16(uint16_t __x) {
    return __x >> 8 | __x << 8;
}

static inline uint32_t __bswap32(uint32_t __x) {
    return __x >> 24 | (__x >> 8) & 0xff00 | ((__x & 0xff00) << 8) | __x << 24;
}

#ifdef __cplusplus
}
#endif

#endif
