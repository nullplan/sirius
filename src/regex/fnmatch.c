#include <fnmatch.h>
#include <stdlib.h>
#include <wctype.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/* main idea for the algorithm (apparently called "Sea of Stars algorithm"):
 * 1st match everything up to the first star.
 * Then match the tail behind the last star.
 * Mismatch in either case is mismatch of the whole pattern.
 *
 * Finally, attempt to match the middle part of the string against the pattern between the first and last star.
 * Match is match of whole pattern. On mismatch, remove the first char of the string and retry.
 * If another star is reached, we commit the entire pattern.
 *
 * A couple other points:
 * FNM_PERIOD is implemented by only allowing a leading '.' in string to match against a literal leading '.' in pattern.
 * FNM_PATHNAME is implemented by breaking pattern into pieces at '/', and matching the string part against the corresponding pattern part.
 * FNM_CASEFOLD is implemented by having a folded version of s0 around.
 * s0 is the widened leading multibyte char of the string. If FNM_CASEFOLD is set, the folded version is the opposite-case of s0, else equal to s0.
 * The pattern then never gets folded. This avoids the error of treating "[A-z]" with casefolding like "[a-z]", and not matching on '\\'.
 */

/* given p pointing at a bracket expression, return nonzero if bracket matches s or sfold. */
static int match_bracket(const char *p, wchar_t s, wchar_t sfold, int flags)
{
    int invert = 0;
    wchar_t start;
    assert(*p == '[');
    p++;
    if (*p == '!') {
        invert = 1;
        p++;
    }
    mbtowc(&start, p, MB_LEN_MAX);
    if (*p == ']' || *p == '-') {
        if (s == *p) return !invert;
        p++;
    }
    while (*p != ']') {
        if (p[0] == '[' && (p[1] == ':' || p[1] == '=' || p[1] == '.')) {
            int type = p[1];
            const char *p0 = p + 2;
            for (p += 3; *p != ']' || p[-1] != type; p++);
            if (type == ':') {
                if (p - p0 < 16) {
                    char buf[16];
                    memcpy(buf, p0, p - p0 - 1);
                    buf[p-p0-1] = 0;
                    wctype_t wct = wctype(buf);
                    if (iswctype(s, wct) || iswctype(sfold, wct)) return !invert;
                }
            } else {
                /* sometimes POSIX can be nice.
                 * In my implementation for example I only have
                 * trivial collation elements and code point collation.
                 * So both collation elements and equivalence classes can
                 * only match a single character each, and it is the one given.
                 *
                 * And if more than one is given, they cannot match at all.
                 */
                int clen = mbtowc(&start, p0, MB_LEN_MAX);
                if (p0 + clen + 1 == p) {
                    if (s == start || sfold == start) return !invert;
                }
            }
        } else if (*p == '-' && p[1] != ']') {
            wchar_t end;
            int clen = mbtowc(&end, p + 1, MB_LEN_MAX);
            if (clen > 0) {
                p += clen;
                if (s - start + 0ul <= end - start + 0ul
                        || sfold - start + 0ul <= end - start + 0ul)
                    return !invert;
            }
        } else if (!(flags & FNM_NOESCAPE) && *p == '\\') {
            int clen = mbtowc(&start, p + 1, MB_LEN_MAX);
            if (clen > 0) {
                p += clen;
                if (s == start || sfold == start) return !invert;
            }
        } else {
            int clen = mbtowc(&start, p, MB_LEN_MAX);
            if (clen > 0) {
                p += clen - 1;
                if (s == start || sfold == start) return !invert;
            }
        }
        p++;
    }
    return invert;
}

static wchar_t pop_string(const char **str, size_t *slen)
{
    wchar_t c;
    int klen = mbtowc(&c, *str, *slen);
    if (klen < 0) {
        c = 0xfffd;
        *str += 1;
        *slen -= 1;
    } else if (klen == 0) {
        c = 0;
    } else {
        *str += klen;
        *slen -= klen;
    }
    return c;
}

static wchar_t casefold(wchar_t x, int flags)
{
    if (!(flags & FNM_CASEFOLD)) return x;
    wchar_t u = towupper(x);
    return u == x? towlower(x) : u;
}

/* tokenizes pattern, returning the next widened MB char, or a negative token.
 * Size of the token is returned in *tlen
 */
#define BRACKET (-2)
#define QUESTION (-3)
#define STAR (-4)
#define UNMATCHABLE (-5)
static int next_token(const char *pattern, size_t plen, size_t *tlen, int flags)
{
    if (!plen || !*pattern) return 0;
    if (*pattern == '?') {
        *tlen = 1;
        return QUESTION;
    }
    if (*pattern == '*') {
        *tlen = 1;
        return STAR;
    }
    if (*pattern == '[') {
        size_t len = 1;
        if (len < plen && pattern[len] == '!') len++;
        if (len < plen && pattern[len] == ']') len++;
        while (len < plen && pattern[len] != ']') {
            if (len + 2 < plen && pattern[len] == '[' 
                    && (pattern[len+1] == ':'
                        || pattern[len+1] == '.'
                        || pattern[len+1] == '='))
            {
                int type = pattern[len+1];
                len += 2;
                while (len < plen && pattern[len] != ']' && pattern[len-1] != type)
                    len++;
            }
            len++;
        }
        if (pattern[len] == ']') {
            *tlen = len + 1;
            return BRACKET;
        }
    }
    int esc = 0;
    if (plen > 1 && *pattern == '\\' && !(flags & FNM_NOESCAPE)) {
        esc = 1;
        pattern++;
        plen--;
    }
    wchar_t rv;
    int len = mbtowc(&rv, pattern, plen);
    if (len < 0) return UNMATCHABLE;
    *tlen = len + esc;
    return rv;
}

static int match(const char *pattern, size_t plen, const char *string, size_t slen, int flags)
{
    size_t tlen;
    int t;
    wchar_t s0, sfold;
    if ((flags & FNM_PERIOD) && slen && *string == '.')
    {
        if (!plen || *pattern != '.') return FNM_NOMATCH;
        string++;
        slen--;
        pattern++;
        plen--;
    }
    while ((t = next_token(pattern, plen, &tlen, flags)) != STAR) {
        if (t == UNMATCHABLE) return FNM_NOMATCH;
        if (t == 0) return (!slen || !*string)? 0 : FNM_NOMATCH;
        s0 = pop_string(&string, &slen);
        sfold = casefold(s0, flags);
        if (s0 == 0) return FNM_NOMATCH;
        if (t == BRACKET && !match_bracket(pattern, s0, sfold, flags)) return FNM_NOMATCH;
        else if (t >= 0 && s0 != t && sfold != t) return FNM_NOMATCH;
        pattern += tlen;
        plen -= tlen;
    }

    /* if we get here, we have just found a star */
    pattern++;
    plen--;
    plen = strnlen(pattern, plen); /* recalculate plen if it was previously unknown. */

    /* now find the tail, i.e. the part of the pattern behind the last star, and count the number of tokens in it. */
    const char *p = pattern;
    size_t p0len = plen;
    const char *tail = pattern;
    const char *endpat = pattern;
    size_t tailchars = 0;       /* number of characters in the tail */
    while ((t = next_token(p, p0len, &tlen, flags))) {
        if (t == UNMATCHABLE) return FNM_NOMATCH;
        else if (t == STAR) {
            tail = p + 1;
            endpat = p;
            tailchars = 0;
        } else {
            tailchars++;
        }
        p += tlen;
        p0len -= tlen;
    }

    slen = strnlen(string, slen);
    /* try to find the pointer to tailchars characters before the end.
     * In ASCII mode, this could just be a pointer subtraction,
     * but I need the loop for UTF-8 mode anyway.
     */
    const char *s = string + slen;
    while (s > string && tailchars) {
        s--;
        if ((unsigned char)*s < 0x80 || (unsigned char)*s >= 0xc0 || MB_CUR_MAX == 1) tailchars--;
    }
    if (tailchars) return FNM_NOMATCH;

    /* now match s against the pattern in tail */
    /* I no longer need to check for UNMATCHABLE since the whole pattern has been parsed once. */
    size_t taillen = plen - (tail - pattern);
    size_t tslen = slen - (s - string);
    slen -= tslen;
    while ((t = next_token(tail, taillen, &tlen, flags))) {
        s0 = pop_string(&s, &tslen);
        if (!s0) return FNM_NOMATCH;
        sfold = casefold(s0, flags);
        if (t == BRACKET && !match_bracket(tail, s0, sfold, flags)) return FNM_NOMATCH;
        else if (t >= 0 && t != s0 && t != sfold) return FNM_NOMATCH;
        tail += tlen;
        taillen -= tlen;
    }

    /* finally, attempt to match middle part */
    while (slen && pattern < endpat) {
        s = string;
        tslen = slen;
        p = pattern;
        plen = endpat - pattern;
        for (;;) {
            t = next_token(p, plen, &tlen, flags);
            if (t == STAR) {
                pattern = p + 1;
                slen -= s - string;
                string = s;
                break;
            }
            s0 = pop_string(&s, &tslen);
            sfold = casefold(s0, flags);
            if ((t == BRACKET && !match_bracket(p, s0, sfold, flags))
                    || (t == QUESTION && !s0)
                    || (t >= 0 && t != s0 && t != sfold)) {
                if (!s0) return FNM_NOMATCH;
                pop_string(&string, &slen);
                break;
            }
            if (!t) return 0;
            p += tlen;
            plen -= tlen;
        }
    }
    if (pattern < endpat) return FNM_NOMATCH;
    return 0;
}

int fnmatch(const char *pattern, const char *string, int flags)
{
    if (flags & FNM_PATHNAME) {
        for (;;) {
            size_t plen = __stridx(pattern, '/');
            size_t slen = __stridx(string, '/');
            if (pattern[plen] != string[slen]) return FNM_NOMATCH;
            if (match(pattern, plen, string, slen, flags)) return FNM_NOMATCH;
            if (!pattern[plen]) return 0;
            pattern += plen + 1;
            string += slen + 1;
        }
    }
    return match(pattern, -1, string, -1, flags);
}
