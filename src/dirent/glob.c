#include <glob.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "libc.h"

static int has_unescaped_globchars(const char *s, size_t n, int flags)
{
    if (flags & GLOB_NOESCAPE)
        return strcspn(s, "*?[") < n;

    int escape = 0;
    while (n--) {
        if (escape)
            escape = 0;
        else if (*s == '\\')
            escape = 1;
        else if (strchr("*?[", *s))
            return 1;
        s++;
    }
    return 0;
}

static size_t unescape_glob(char *dst, const char *src, size_t n, int flags)
{
    char *odst = dst;
    if (flags & GLOB_NOESCAPE) {
        memcpy(dst, src, n);
        return n;
    }

    while (n) {
        if (*src == '\\' && n > 1) {
            *dst++ = src[1];
            src += 2;
            n -= 2;
        } else {
            *dst++ = *src++;
            n--;
        }
    }
    return dst - odst;
}

struct pathlist {
    size_t capacity;
    size_t n;
    char **paths;
};

static int push_back_path(struct pathlist *pl, char *path)
{
    if (pl->n == pl->capacity) {
        size_t attempt = MAX(16, pl->n + pl->n/2);
        char **p = realloc(pl->paths, attempt * sizeof (char *));
        if (!p) return -1;
        pl->capacity = attempt;
        pl->paths = p;
    }
    pl->paths[pl->n++] = path;
    return 0;
}

static int default_errfunc(const char *path, int err)
{
    if (err == ENOENT || err == ENOTDIR) return 0;
    return 1;
}

static int process_name(struct pathlist *out, const struct pathlist *candidates, int (*errfunc)(const char *, int), int flags, const char *name, size_t n)
{
    out->paths = 0;
    out->n = 0;
    out->capacity = 0;

    if (!has_unescaped_globchars(name, n, flags)) {
        char unescaped[n];
        size_t len = unescape_glob(unescaped, name, n, flags);
        out->paths = malloc(candidates->n * sizeof (char *));
        if (!out->paths) return GLOB_NOSPACE;
        out->capacity = candidates->n;
        for (size_t i = 0; i < candidates->n; i++) {
            size_t cl = strlen(candidates->paths[i]);
            out->paths[out->n] = malloc(cl + len + 3); /* one is extra for GLOB_MARK */
            if (!out->paths[out->n]) return GLOB_NOSPACE;
            if (cl) {
                memcpy(out->paths[out->n], candidates->paths[i], cl);
                if (out->paths[out->n][cl - 1] != '/')
                    out->paths[out->n][cl++] = '/';
            }
            memcpy(out->paths[out->n] + cl, unescaped, len);
            out->paths[out->n][cl + len] = 0;
            out->n++;
        }
    } else {
        for (size_t i = 0; i < candidates->n; i++) {
            DIR *d = opendir(candidates->paths[i][0]? candidates->paths[i] : ".");
            if (!d) {
                int rv = errfunc(candidates->paths[i], errno);
                if (rv || (flags & GLOB_ERR)) return GLOB_ABORTED;
            } else {
                struct dirent *de;
                char pattern[n + 1];
                int fnm_flags = FNM_PERIOD;
                if (flags & GLOB_NOESCAPE) fnm_flags |= FNM_NOESCAPE;
                memcpy(pattern, name, n);
                pattern[n] = 0;
                int saved_errno = errno;
                errno = 0;
                while ((de = readdir(d))) {
                    if (strcmp(de->d_name, ".") && strcmp(de->d_name, "..") && !fnmatch(pattern, de->d_name, fnm_flags)) {
                        size_t cl = strlen(candidates->paths[i]);
                        size_t nl = strlen(de->d_name);
                        char *path = malloc(cl + nl + 3);
                        if (!path) {
                            closedir(d);
                            return GLOB_NOSPACE;
                        }
                        if (cl) {
                            memcpy(path, candidates->paths[i], cl);
                            if (path[cl - 1] != '/')
                                path[cl++] = '/';
                        }
                        memcpy(path + cl, de->d_name, nl + 1);
                        if (push_back_path(out, path)) {
                            free(path);
                            closedir(d);
                            return GLOB_NOSPACE;
                        }
                    }
                }
                closedir(d);
                if (errno) {
                    int rv = errfunc(candidates->paths[i], errno);
                    if (rv || (flags & GLOB_ERR)) return GLOB_ABORTED;
                }
                errno = saved_errno;
            }
        }
    }

    return 0;
}

static int pathcmp(const void *a, const void *b)
{
    return strcoll(*(char **)a, *(char **)b);
}

static void freelist(struct pathlist *l)
{
    for (size_t i = 0; i < l->n; i++)
        free(l->paths[i]);
    free(l->paths);
    l->n = l->capacity = 0;
    l->paths = 0;
}

int glob(const char *restrict pat, int flags, int (*errfunc)(const char *, int), glob_t *restrict g)
{
    if (!errfunc) errfunc = default_errfunc;
    size_t off = flags & GLOB_DOOFFS? g->gl_offs : 0;
    if (!(flags & GLOB_APPEND)) {
        g->gl_offs = off;
        g->gl_pathc = 0;
        g->gl_pathv = 0;
    }

    struct pathlist noalloc, a1 = {0}, a2 = {0};
    struct pathlist *result = &noalloc;
    noalloc.n = 0;
    int rv = GLOB_NOMATCH;
    char *p = "/";
    if (*pat) {
        noalloc.n = 1;
        noalloc.paths = &p;
        if (*pat != '/')
            p++;
        else
            rv = 0;

        struct pathlist *in = &noalloc;
        const char *a = pat;
        const char *z;
        for (; *a; a = z + !!*z) {
            while (*a == '/') a++;
            if (!*a) break;
            z = a + __stridx(a, '/');
            if (in == &a2) result = &a1;
            else result = &a2;
            rv = process_name(result, in, errfunc, flags, a, z - a);
            if (rv) break;
            if (in != &noalloc) freelist(in);
            in = result;
        }
        if (!rv && a[-1] == '/') {
            for (size_t i = 0; i < result->n; i++)
            {
                size_t len = strlen(result->paths[i]);
                if (len && result->paths[i][len - 1] != '/') {
                    result->paths[i][len] = '/';
                    result->paths[i][len + 1] = 0;
                }
            }
        }
    }

    if (result != &a1) freelist(&a1);
    if (result != &a2) freelist(&a2);
    if (rv == GLOB_NOSPACE || rv == GLOB_ABORTED) {
        if (result != &noalloc) freelist(result);
        return rv;
    }
    for (size_t i = 0; i < result->n; ) {
        struct stat st;
        if (lstat(result->paths[i], &st)) {
            if (errfunc(result->paths[i], errno) || (flags & GLOB_ERR)) {
                if (result != &noalloc)
                    freelist(result);
                return GLOB_ABORTED;
            }
            if (result != &noalloc) free(result->paths[i]);
            result->paths[i] = result->paths[--(result->n)];
        } else {
            if ((flags & GLOB_MARK) && S_ISDIR(st.st_mode)) {
                size_t len = strlen(result->paths[i]);
                if (result->paths[i][len - 1] != '/') {
                    result->paths[i][len] = '/';
                    result->paths[i][len + 1] = 0;
                }
            }
            i++;
        }
    }
    if (result->n == 0) {
        if (result != &noalloc)
            freelist(result);
        if (!(flags & GLOB_NOCHECK)) {
            return GLOB_NOMATCH;
        }
        result = &noalloc;
        noalloc.n = 1;
        noalloc.paths = (void *)&pat;
    }

    if (result == &noalloc) {
        a1.paths = malloc(result->n * sizeof (char *));
        if (!a1.paths)
            return GLOB_NOSPACE;

        for (a1.n = 0; a1.n < result->n; a1.n++)
        {
            a1.paths[a1.n] = strdup(result->paths[a1.n]);
            if (!a1.paths[a1.n]) break;
        }
        if (a1.n < result->n) {
            freelist(&a1);
            return GLOB_NOSPACE;
        }
        result = &a1;
    }

    if (!(flags & GLOB_NOSORT) && result->n > 1)
        qsort(result->paths, result->n, sizeof (char *), pathcmp);

    char **final = realloc(g->gl_pathv, (off + g->gl_pathc + result->n + 1) * sizeof (char *));
    if (!final) {
        freelist(result);
        return GLOB_NOSPACE;
    }
    g->gl_pathv = final;
    memset(g->gl_pathv, 0, off * sizeof (char *));
    memcpy(g->gl_pathv + off + g->gl_pathc, result->paths, result->n * sizeof (char *));
    g->gl_pathv[off + g->gl_pathc + result->n] = 0;
    g->gl_pathc += result->n;
    free(result->paths);
    return 0;
}
