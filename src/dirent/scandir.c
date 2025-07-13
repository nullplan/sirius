#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

static int wrap_cmp(const void *a, const void *b, void *c)
{
    int (*cmp)(const struct dirent **, const struct dirent **) = c;
    return cmp((void *)a, (void *)b);
}

int scandir(const char *name, struct dirent ***retbuf, int (*filter)(const struct dirent *), int (*cmp)(const struct dirent **, const struct dirent **))
{
    struct dirent **buffer = 0;
    size_t buflen = 0, capacity = 0;

    DIR *d = opendir(name);
    if (!d) return -1;

    struct dirent *de;
    while ((de = readdir(d))) {
        if (filter && !filter(de)) continue;
        if (buflen == INT_MAX) {
            errno = EOVERFLOW;
            goto error;
        }
        if (buflen == capacity) {
            capacity = capacity < 11? 16 : capacity + capacity/2;
            if (capacity < buflen) {
                errno = EOVERFLOW;
                goto error;
            }
            void *p = realloc(buffer, capacity * sizeof (struct dirent *));
            if (!p) goto error;
            buffer = p;
        }
        buffer[buflen] = malloc(de->d_reclen);
        if (!buffer[buflen]) goto error;
        memcpy(buffer[buflen++], de, de->d_reclen);
    }
    closedir(d);

    if (cmp)
        qsort_r(buffer, buflen, sizeof (struct dirent *), wrap_cmp, cmp);
    *retbuf = buffer;
    return buflen;

error:
    for (size_t i = 0; i < buflen; i++)
        free(buffer[i]);
    free(buffer);
    closedir(d);
    return -1;
}
