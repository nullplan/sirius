#ifndef WCHAR_H
#define WCHAR_H

#include "../../include/wchar.h"

#define UTF_START   0xC2
#define UTF_END     0xF4
extern hidden const unsigned __utftab[UTF_END - UTF_START + 1];
#define TO_CODEUNIT(x)  (0xdfff & (signed char)(x))
#define IS_CODEUNIT(wc) ((wc) - 0xdf80u < 0x80u)
#define FROM_CODEUNIT(wc)   ((wc) & 0xff)

extern hidden int __is_in_set(wint_t, const unsigned char *, size_t);
extern hidden unsigned __read_uleb128(const unsigned char **);
extern hidden size_t __wcsidx(const wchar_t *, wchar_t);
#endif
