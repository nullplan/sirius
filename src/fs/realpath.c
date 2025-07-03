#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

char *realpath(const char *restrict path, char *restrict result)
{
    if (!path) {
        errno = EINVAL;
        return 0;
    }
    if (!*path) {
        errno = ENOENT;
        return 0;
    }

    char buffer[PATH_MAX];              /* readlink buffer */
    char stack[PATH_MAX + 1];           /* buffer for resolved and to_be_resolved paths */
    char *tail = stack;                 /* end of resolved path */
    char *to_be_resolved;               /* start of to_be_resolved path */
    size_t len = strnlen(path, PATH_MAX);       /* temp for various lengths */
    int known_dir = 1;                  /* bool for whether resolved is known to be a dir. Since we always start at / or ., it starts at 1 */
    int links = SYMLOOP_MAX;            /* link counter to return ELOOP at some point */
    int cdup = 0;                       /* counter of up-dir components at start of resolved */
    if (len >= PATH_MAX) {
        errno = ENAMETOOLONG;
        return 0;
    }

    to_be_resolved = memcpy(stack + sizeof stack - len - 1, path, len + 1);
    if (*to_be_resolved == '/') {
        to_be_resolved++;
        *tail++ = '/';
    }
    *tail = 0;

    for (;;) {
        to_be_resolved += strspn(to_be_resolved, "/");
        if (!*to_be_resolved)
            break;
        len = __stridx(to_be_resolved, '/');
        char *end = to_be_resolved + len;
        if (len == 1 && *to_be_resolved == '.') {
            if (known_dir) {
                to_be_resolved = end;
                continue;
            }
            /* this is a bit sneaky and might require explanation:
             * I want to append a / to the resolved path if it doesn't already
             * end in one. So far, so clear. But what if tail==stack?
             * Turns out that tail==stack always implies known_dir,
             * so in that case we never get here.
             */
            if (tail[-1] != '/') {
                tail[0] = '/';
                tail[1] = 0;
            }
        } else if (len == 2 && to_be_resolved[0] == '.' && to_be_resolved[1] == '.') {
            to_be_resolved = end;
            if (tail == stack + 3 * cdup) {
                memcpy(tail, "../", 4);
                tail += 3;
                cdup++;
                known_dir = 1;
                continue;
            }
            if (known_dir) goto remove_component;
            if (tail[-1] != '/') {
                tail[0] = '/';
                tail[1] = 0;
            }
        } else {
            if (tail > stack && tail[-1] != '/')
                *tail++ = '/';
            memmove(tail, to_be_resolved, len);
            tail[len] = 0;
            to_be_resolved += len;
        }
        ssize_t rv = readlink(stack, buffer, sizeof buffer);
        if (rv == -1 && errno == EINVAL) { /* file exists and is no symlink */
            if (*tail == '/') { /* this was only a dir test for a . or .. component */
                *tail = 0; /* so strip off the / we appended */
                known_dir = 1; /* and record that this really is a directory */
                if (len == 2) {
                    /* and for .., remove the highest component */
remove_component:
                    while (tail > stack && tail[-1] != '/')
                        tail--;
                    if (tail > stack) tail--;
                    *tail = 0;
                }
            } else { /* this was a normal component */
                tail += len;
                known_dir = 0;
            }
        } else if (rv == -1) return 0; /* file lookup failed, so return failure */
        else if (!links--) {
            errno = ELOOP;
            return 0;
        } else {
            known_dir = 1;
            if (*buffer == '/') {
                tail = stack + 1;
                tail[-1] = '/';
                tail[0] = 0;
            } else if (tail > stack) {
                *--tail = 0;
            } else
                *tail = 0;
            if (rv >= to_be_resolved - tail) {
                errno = ENAMETOOLONG;
                return 0;
            }
            to_be_resolved -= rv;
            memcpy(to_be_resolved, buffer, rv);
        }
    }
    if (to_be_resolved[-1] == '/' && !known_dir) {
        if (tail[-1] != '/') {
            tail[0] = '/';
            tail[1] = 0;
        }
        int rv = readlink(stack, buffer, sizeof buffer);
        if (rv == -1) return 0;
        *tail = 0;
    }
    if (*stack != '/') {
        if (!getcwd(buffer, sizeof buffer)) return 0;
        char *dirtail = buffer + strlen(buffer);
        char *start = stack + 3 * cdup;
        while (cdup--) {
            while (dirtail > buffer && dirtail[-1] != '/')
                dirtail--;
            if (dirtail > buffer) dirtail--;
        }
        if ((dirtail - buffer) + (tail - start) >= PATH_MAX) {
            errno = ENAMETOOLONG;
            return 0;
        }
        memmove(stack + (dirtail - buffer) + 1, start, (tail - start) + 1);
        memcpy(stack, buffer, dirtail - buffer);
        stack[dirtail - buffer] = '/';
    }
    return result? memcpy(result, stack, tail - stack + 1) : strdup(stack);
}
