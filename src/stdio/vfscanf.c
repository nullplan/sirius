#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

static void *nth_ptr(va_list ap, int n)
{
    va_list ap2;
    va_copy(ap2, ap);
    while (n) va_arg(ap2, void *);
    void *rv = va_arg(ap2, void *);
    va_end(ap2);
    return rv;
}

enum lengthmod { BARE, HPRE, HHPRE, LPRE, LLPRE, JPRE, ZTPRE, CLPRE, W8, W16, W32, W64, WF8, WF16, WF32, WF64 };
static const uintmax_t signed_limit[] = {
           INT_MIN,
          SHRT_MIN,
         SCHAR_MIN,
          LONG_MIN,
         LLONG_MIN,
        INTMAX_MIN,
       PTRDIFF_MIN,
    0,
          INT8_MIN,
         INT16_MIN,
         INT32_MIN,
         INT64_MIN,
     INT_FAST8_MIN,
    INT_FAST16_MIN,
    INT_FAST32_MIN,
    INT_FAST64_MIN
};
static const uintmax_t unsigned_limit[] = {
    UINT_MAX,
    USHRT_MAX,
    UCHAR_MAX,
    ULONG_MAX,
    ULLONG_MAX,
    UINTMAX_MAX,
    SIZE_MAX,
    0,
    UINT8_MAX,
    UINT16_MAX,
    UINT32_MAX,
    UINT64_MAX,
    UINT_FAST8_MAX,
    UINT_FAST16_MAX,
    UINT_FAST32_MAX,
    UINT_FAST64_MAX
};
static const int float_bits[] = {
    FLT_MANT_DIG,
    0, 0,
    DBL_MANT_DIG,
    0, 0, 0,
    LDBL_MANT_DIG
};
static const int float_emin[] = {
    FLT_MIN_EXP,
    0, 0,
    DBL_MIN_EXP,
    0, 0, 0,
    LDBL_MIN_EXP
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
    }
}

static void set_float_ptr(void *p, enum lengthmod len, long double val)
{
    switch (len) {
        case BARE:      *(float *)p = val; break;
        case LPRE:      *(double *)p = val; break;
        case CLPRE:     *(long double *)p = val; break;
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
            struct modifier *m = find_mod(fmt);
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
            if (!suppress) {
                if (argpos != -1)
                    p = nth_ptr(ap);
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
                    {
                        long double val = __floatscan(f, float_bits[len], float_emin[len]);
                        if (!shcnt(f)) goto quit;
                        if (p) set_float_ptr(p, len, val);
                        conversions++;
                    }
                    break;
                case 'C':
                case 'S':
                    len = LPRE;
                    /* fall through */
                case 'c':
                case 's':
                case '[':
                    /* XXX */
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
