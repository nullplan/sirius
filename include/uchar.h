#ifndef __UCHAR_H
#define __UCHAR_H

#define __NEED_mbstate_t
#define __NEED_size_t
#include <alltypes.h>
#include <features.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef unsigned char char8_t;
typedef unsigned short char16_t;
typedef unsigned char32_t;
#endif

size_t mbrtoc8(char8_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);
size_t c8rtomb(char *__restrict, char8_t, mbstate_t *__restrict);
size_t mbrtoc16(char16_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);
size_t c16rtomb(char *__restrict, char16_t, mbstate_t *__restrict);
size_t mbrtoc32(char32_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);
size_t c32rtomb(char *__restrict, char32_t, mbstate_t *__restrict);

#ifdef __cplusplus
}
#endif
#endif
