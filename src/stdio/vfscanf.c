#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

static void *nth_ptr(va_list ap, int n)
{
    va_list ap2;
    va_copy(ap2, ap);
    while (n--) va_arg(ap2, void *);
    void *rv = va_arg(ap2, void *);
    va_end(ap2);
    return rv;
}

enum lengthmod { BARE, HPRE, HHPRE, LPRE, LLPRE, JPRE, ZTPRE, CLPRE, W8, W16, W32, W64, WF8, WF16, WF32, WF64 };
static const uintmax_t signed_limit[] = {
    [BARE] =           INT_MIN,
    [HPRE] =          SHRT_MIN,
    [HHPRE] =        SCHAR_MIN,
    [LPRE] =          LONG_MIN,
    [LLPRE] =        LLONG_MIN,
    [JPRE] =        INTMAX_MIN,
    [ZTPRE] =      PTRDIFF_MIN,
    [W8] =            INT8_MIN,
    [W16] =          INT16_MIN,
    [W32] =          INT32_MIN,
    [W64] =          INT64_MIN,
    [WF8] =      INT_FAST8_MIN,
    [WF16] =    INT_FAST16_MIN,
    [WF32] =    INT_FAST32_MIN,
    [WF64] =    INT_FAST64_MIN
};
static const uintmax_t unsigned_limit[] = {
    [BARE] =      UINT_MAX,
    [HPRE] =      USHRT_MAX,
    [HHPRE] =     UCHAR_MAX,
    [LPRE] =      ULONG_MAX,
    [LLPRE] =     ULLONG_MAX,
    [JPRE] =      UINTMAX_MAX,
    [ZTPRE] =     SIZE_MAX,
    [W8] =        UINT8_MAX,
    [W16] =       UINT16_MAX,
    [W32] =       UINT32_MAX,
    [W64] =       UINT64_MAX,
    [WF8] =       UINT_FAST8_MAX,
    [WF16] =      UINT_FAST16_MAX,
    [WF32] =      UINT_FAST32_MAX,
    [WF64] =      UINT_FAST64_MAX
};
static const int float_bits[] = {
    [BARE] = FLT_MANT_DIG,
    [LPRE] = DBL_MANT_DIG,
    [CLPRE] = LDBL_MANT_DIG,
};
static const int float_emin[] = {
    [BARE] =  FLT_MIN_EXP,
    [LPRE] =  DBL_MIN_EXP,
    [CLPRE] = LDBL_MIN_EXP
};

static void set_int_ptr(void *p, enum lengthmod len, intmax_t val)
{
    switch (len) {
        case BARE:              *(int *)p = val; break;
        case HPRE:              *(short *)p = val; break;
        case HHPRE:             *(signed char *)p = val; break;
        case LPRE:              *(long *)p = val; break;
        case LLPRE:             *(long long *)p = val; break;
        case JPRE:              *(intmax_t *)p = val; break;
        case ZTPRE:             *(ptrdiff_t *)p = val; break;
        case W8:                *(int8_t *)p = val; break;
        case W16:               *(int16_t *)p = val; break;
        case W32:               *(int32_t *)p = val; break;
        case W64:               *(int64_t *)p = val; break;
        case WF8:               *(int_fast8_t *)p = val; break;
        case WF16:              *(int_fast16_t *)p = val; break;
        case WF32:              *(int_fast32_t *)p = val; break;
        case WF64:              *(int_fast64_t *)p = val; break;
        default:                abort();
    }
}

static void set_uint_ptr(void *p, enum lengthmod len, uintmax_t val)
{
    switch (len) {
        case BARE:              *(unsigned int *)p = val; break;
        case HPRE:              *(unsigned short *)p = val; break;
        case HHPRE:             *(unsigned char *)p = val; break;
        case LPRE:              *(unsigned long *)p = val; break;
        case LLPRE:             *(unsigned long long *)p = val; break;
        case JPRE:              *(uintmax_t *)p = val; break;
        case ZTPRE:             *(size_t *)p = val; break;
        case W8:                *(uint8_t *)p = val; break;
        case W16:               *(uint16_t *)p = val; break;
        case W32:               *(uint32_t *)p = val; break;
        case W64:               *(uint64_t *)p = val; break;
        case WF8:               *(uint_fast8_t *)p = val; break;
        case WF16:              *(uint_fast16_t *)p = val; break;
        case WF32:              *(uint_fast32_t *)p = val; break;
        case WF64:              *(uint_fast64_t *)p = val; break;
        default:                abort();
    }
}

static void set_float_ptr(void *p, enum lengthmod len, long double val)
{
    switch (len) {
        case BARE:      *(float *)p = val; break;
        case LPRE:      *(double *)p = val; break;
        case CLPRE:     *(long double *)p = val; break;
        default:        abort();
    }
}

static const struct modifier {
    unsigned char len;
    unsigned char mod;
    char str[4];
} modifiers[] = {
    /* must stay sorted by length descending to match longer strings before shorter ones. */
    {4, WF16, "wf16"},
    {4, WF32, "wf32"},
    {4, WF64, "wf64"},
    {3, W16, "w16"},
    {3, W32, "w32"},
    {3, W64, "w64"},
    {3, WF8, "wf8"},
    {2, W8, "w8"},
    {2, HHPRE, "hh"},
    {2, LLPRE, "ll"},
    {1, HPRE, "h"},
    {1, LPRE, "l"},
    {1, ZTPRE, "z"},
    {1, ZTPRE, "t"},
    {1, CLPRE, "L"},
    {0, BARE, ""},
};

static const struct modifier *find_mod(const char *fmt)
{
    for (const struct modifier *m = modifiers;; m++)
    {
        if (!memcmp(fmt, m->str, m->len)) return m;
    }
}

int vfscanf(FILE *restrict f, const char *restrict fmt, va_list ap)
{
    int conversions = 0;
    off_t consumed = 0;
    unsigned char scanset[257];

    __FLOCK(f);
    shlim(f, 0);
    while (*fmt)
    {
        if (isspace(*fmt)) {
            int c = shgetc(f);
            while (isspace(c))
                c = shgetc(f);
            if (c != EOF)
                shungetc(f);
            fmt++;
        } else if (*fmt != '%' || fmt[1] == '%') {
            int c = shgetc(f);
            if (*fmt == '%')
                while (isspace(c))
                    c = shgetc(f);
            if (c != *fmt) {
                if (c != EOF) shungetc(f);
                break;
            }
            if (*fmt == '%') fmt++;
            fmt++;
        } else {
            int argpos = -1;
            int suppress = 0;
            int width = 0;
            int allocate = 0;
            enum lengthmod len = BARE;
            fmt++;
            if ((unsigned)(*fmt - '1') < NL_ARGMAX && fmt[1] == '$') {
                argpos = *fmt - '1';
                fmt += 2;
            }
            if (*fmt == '*') {
                fmt++;
                suppress = 1;
            }
            if (*fmt - '1' < 9u) {
                width = strtol(fmt, (char **)&fmt, 10);
            }
            if (*fmt == 'm') {
                allocate = 1;
                fmt++;
            }
            const struct modifier *m = find_mod(fmt);
            fmt += m->len;
            len = m->mod;
            int skipws = *fmt != '[' && *fmt != 'c' && *fmt != 'C' && *fmt != 'n';
            if (skipws) {
                int c = shgetc(f);
                while (isspace(c))
                    c = shgetc(f);
                if (c != EOF)
                    shungetc(f);
            }
            consumed += shcnt(f);
            shlim(f, width);
            int base;
            uintmax_t limit;
            void *p = 0;
            mbstate_t mbs = {0};
            if (!suppress) {
                if (argpos != -1)
                    p = nth_ptr(ap, argpos);
                else
                    p = va_arg(ap, void *);
            }
            switch (*fmt) {
                case 'b':
                case 'd':
                case 'i':
                case 'o':
                case 'u':
                case 'x':
                case 'p':
                    switch (*fmt) {
                        case 'b':
                            base = 2;
                            limit = unsigned_limit[len];
                            break;
                        case 'd':
                            base = 10;
                            limit = signed_limit[len];
                            break;
                        case 'i':
                            base = 0;
                            limit = signed_limit[len];
                            break;
                        case 'o':
                            base = 8;
                            limit = unsigned_limit[len];
                            break;
                        case 'u':
                            base = 10;
                            limit = unsigned_limit[len];
                            break;
                        case 'p':
                            len = LPRE;
                            /* fall through */
                        case 'x':
                        case 'X':
                            base = 16;
                            limit = unsigned_limit[len];
                            break;
                    }
                    {
                        uintmax_t val = __intscan(f, base, limit);
                        if (!shcnt(f)) goto quit;
                        if (p) {
                            if (!(limit & 1)) set_int_ptr(p, len, val);
                            else set_uint_ptr(p, len, val);
                            conversions++;
                        }
                    }
                    break;
                case 'a': case 'e': case 'f': case 'g':
                case 'A': case 'E': case 'F': case 'G':
                    {
                        long double val = __floatscan(f, 0, float_bits[len], float_emin[len]);
                        if (!shcnt(f)) goto quit;
                        if (p) set_float_ptr(p, len, val);
                        conversions++;
                    }
                    break;
                case 'C':
                    len = LPRE;
                    /* fall through */
                case 'c':
                    if (!width) {
                        shlim(f, 1);
                        width = 1;
                    }
                    if (len == LPRE)
                    {
                        wchar_t *str;
                        if (allocate) {
                            str = malloc(width * sizeof (wchar_t));
                            if (!str) goto quit;
                            *(wchar_t **)p = str;
                        } else
                            str = p;
                        int c = shgetc(f);
                        while (c != EOF) {
                            char b = c;
                            size_t len = mbrtowc(str, &b, 1, &mbs);
                            if (len == -1) {
                                if (allocate) free(*(wchar_t **)p);
                                goto quit;
                            }
                            if (len != -2)
                                str++;
                            c = shgetc(f);
                        }
                        if (!mbsinit(&mbs)) {
                            if (allocate) free(*(wchar_t **)p);
                            goto quit;
                        }
                        conversions++;
                        break;
                    }
                    {
                        char *str;
                        if (allocate) {
                            str = malloc(width + 1);
                            if (!str) goto quit;
                            *(char **)p = str;
                        } else
                            str = p;
                        int c = shgetc(f);
                        size_t i = 0;
                        while (c != EOF) {
                            *str++ = c;
                            c = shgetc(f);
                            i++;
                        }
                        if (i < width) {
                            if (allocate) free(*(char **)p);
                            goto quit;
                        }
                        conversions++;
                    }
                    break;

                case 'S':
                    len = LPRE;
                    /* fall through */
                case 's':
                    memset(scanset + 1, 1, 256);
                    for (int i = 0; i < 256; i++)
                        if (isspace(i))
                            scanset[i+1] = 0;

                    if (0) {
                case '[':;
                        int invert = 0;
                        fmt++;
                        if (*fmt == '^') {
                            fmt++;
                            memset(scanset + 1, 1, 256);
                            invert = 1;
                        } else {
                            memset(scanset + 1, 0, 256);
                        }
                        do scanset[(unsigned char)*fmt++ + 1] = !invert;
                        while (*fmt != ']');
                    }
                    {
                        char *str = 0;
                        size_t capa = 0;
                        size_t len = 0;
                        if (allocate) {
                            str = malloc(16);
                            if (!str) goto quit;
                            *(char **)p = str;
                            capa = 16;
                        } else
                            str = p;
                        int c = shgetc(f);
                        while (scanset[c+1]) {
                            if (allocate && capa == len + 1) {
                                size_t attempt = capa + capa / 2;
                                char *new = realloc(str, attempt);
                                if (!new) {
                                    free(str);
                                    goto quit;
                                }
                                str = new;
                                capa = attempt;
                            }
                            str[len++] = c;
                            c = shgetc(f);
                        }
                        if (c != EOF)
                            shungetc(f);
                        str[len] = 0;
                        conversions++;
                    }
                    break;
                case 'n':
                    if (p) set_uint_ptr(p, len, consumed);
                    break;
            }
            fmt++;
        }
    }
quit:
    __FUNLOCK(f);
    return conversions;
}
