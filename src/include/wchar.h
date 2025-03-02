#ifndef WCHAR_H
#define WCHAR_H

#include "../../include/wchar.h"

#define UTF_START   0xC2
#define UTF_END     0xF4
extern hidden unsigned __utftab[UTF_END - UTF_START + 1];
#define TO_CODEUNIT(x)  (0xdfff & (signed char)(x))
#define IS_CODEUNIT(wc) ((wc) - 0xdf80u < 0x80u)
#define FROM_CODEUNIT(wc)   ((wc) & 0xff)
#endif
