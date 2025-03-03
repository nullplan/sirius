#include <fnmatch.h>
#include <stdlib.h>
#include <wctype.h>
#include <string.h>
#include <errno.h>

static const struct charclass {
    const char *name;
    int (*fn)(wint_t);
} charclasses[] = {
    {"[:alpha:]", iswalpha},
    {"[:alnum:]", iswalnum},
    {"[:blank:]", iswblank},
    {"[:cntrl:]", iswcntrl},
    {"[:digit:]", iswdigit},
    {"[:graph:]", iswgraph},
    {"[:lower:]", iswlower},
    {"[:print:]", iswprint},
    {"[:punct:]", iswpunct},
    {"[:space:]", iswspace},
    {"[:upper:]", iswupper},
    {"[:xdigit:]", iswxdigit},
};

static const struct charclass *get_charclass(const char *name)
{
    for (const struct charclass *c = charclasses; c < charclasses + sizeof charclasses / sizeof *charclasses; c++)
        if (!strncmp(name, c->name, strlen(c->name)))
            return c;
    return 0;
}

static int match(const char *pattern, const char *string, int flags, int first)
{
    /* if string is empty, pattern can only match if it only consists of stars */
    if (!*string) {
        while (*pattern == '*') pattern++;
        return *pattern ? FNM_NOMATCH : 0;
    }

    wchar_t p0, s0;
    int n = mbtowc(&p0, pattern, MB_LEN_MAX);
    if (!n) p0 = 0;
    else if (n == -1) return EILSEQ;
    n = mbtowc(&s0, string, MB_LEN_MAX);
    if (n == -1) return EILSEQ;
    if (flags & FNM_CASEFOLD) {
        p0 = towlower(p0);
        s0 = towlower(s0);
    }
    if (s0 == '/' && (flags & FNM_PATHNAME)) {
        if (p0 != '/') return 1;
        return match(pattern, string + n, flags, 1);
    }
    if (s0 == '.' && (flags & FNM_PERIOD) && first) {
        if (p0 != '.') return 1;
        return match(pattern, string + n, flags, 0);
    }

    int invert = 0;
    wchar_t start;
    wchar_t end;
    int bracket_matches = 0;
    const char *p = pattern;
    switch (p0) {
        case '?':
            /* since we already know string to not be empty */
            return match(pattern, string + n, flags, 0);
        case '*':
            // e.g. pattern = "*/*.c", string = "random/rand.c"
            // I try to match "/*.c" with "random/rand.c", and when that doesn't work,
            // I try to match "/*.c" with "andom/rand.c", and so on.
            // Suppose we have FNM_PATHNAME, then at some point I will reach the /.
            // I terminate the loop at that point, and then finally return the match
            // of "/*.c" against "/rand.c".
            while (s0 && (!(flags & FNM_PATHNAME) || s0 != '/'))
            {
                int rv = match(pattern, string, flags, first);
                if (rv != FNM_NOMATCH)
                    return rv;
                string += n;
                first = 0;
                n = mbtowc(&s0, string, MB_LEN_MAX);
                if (!n) s0 = 0;
                else if (n == -1) return EILSEQ;
            }
            return match(pattern, string, flags, first);
        case '[':
            /* a bracket expressions is only concerned with s0,
             * so just advance string to the next character.
             * If bracket matches, we can continue there.
             */
            string += n;
            if (*p == '!') {
                p++;
                invert = 1;
            }
            do {
                n = mbtowc(&p0, p, MB_LEN_MAX);
                if (n == -1) return EILSEQ;
                else if (n == 0) goto badpat;
                p += n;
                if (p0 == '[' && (*p == '.' || *p == ':' || *p == '=')) {
                    if (*p == '.')
                    {
                        /* collation element. Those are not supported, so we just skip over it. */
                        p++;
                        while (*p && p[-2] != '.' && p[-1] != ']') p++;
                        if (p[-1] != ']') goto badpat;
                    } else if (*p == ':') {
                        /* char class. Find charclass in list, apply match as needed. */
                        const struct charclass *c = get_charclass(p-1);
                        if (!c) goto badpat;
                        if ((!!c->fn(s0)) ^ invert) bracket_matches = 1;
                        p += strlen(c->name) - 1;
                    } else {
                        /* equivalence class. Luckily, each wchar_t only represents itself in all supported collations. */
                        n = mbtowc(&p0, p + 1, MB_LEN_MAX);
                        if (n <= 0 || p[1+n] != '=' || p[2+n] != ']') goto badpat;
                        if (flags & FNM_CASEFOLD) p0 = towlower(p0);
                        if ((s0 == p0) ^ invert) bracket_matches = 1;
                        p += 2 + n;
                    }
                } else if (*p == '-' && p[1] != ']') {
                    /* character range */
                    start = p0;
                    n = mbtowc(&end, p + 1, MB_LEN_MAX);
                    if (n <= 0) goto badpat;
                    p += 1 + n;
                    if (flags & FNM_CASEFOLD) {
                        start = towlower(start);
                        end = towlower(end);
                    }
                    if ((start <= s0 && s0 <= end) ^ invert) bracket_matches = 1;
                } else {
                    /* single character */
                    if (flags & FNM_CASEFOLD)
                        p0 = towlower(p0);
                    if ((s0 == p0) ^ invert) bracket_matches = 1;
                }
            } while (*p != ']');
            if (!bracket_matches) return 1;
            return match(p + 1, string, flags, 0);

badpat:
            if (s0 != '[') return 1;
            return match(pattern, string, flags, 0);

        default:
            if (s0 != p0) return 1;
            return match(pattern, string + n, flags, 0);
    }
}

int fnmatch(const char *pattern, const char *string, int flags)
{
    return match(pattern, string, flags, 1);
}
