#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>

static int wrap_cmp(const void *a, const void *b, void *c)
{
    int (*cmp)(const struct dirent **, const struct dirent **) = c;
    return cmp((void *)a, (void *)b);
}

int scandir(const char *name, struct dirent ***retbuf, int (*filter)(const struct dirent *), int (*cmp)(const struct dirent **, const struct dirent **))
{
    struct dirent **buffer = 0;
    size_t buflen = 0, capacity = 0;
    int cs;

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    DIR *d = opendir(name);
    if (!d) goto error;

    struct dirent *de;
    int saved_errno = errno;
    while ((errno = 0), (de = readdir(d))) {
        errno = saved_errno;
        if (filter && !filter(de)) continue;
        if (buflen == INT_MAX) {
            errno = EOVERFLOW;
            goto error_free;
        }
        if (buflen == capacity) {
            capacity = capacity < 11? 16 : capacity + capacity/2;
            if (capacity < buflen) {
                errno = EOVERFLOW;
                goto error_free;
            }
            void *p = realloc(buffer, capacity * sizeof (struct dirent *));
            if (!p) goto error_free;
            buffer = p;
        }
        buffer[buflen] = malloc(de->d_reclen);
        if (!buffer[buflen]) goto error_free;
        memcpy(buffer[buflen++], de, de->d_reclen);
    }
    int err = errno;
    closedir(d);
    if (err) {
        errno = err;
        goto error_free;
    }

    errno = saved_errno;
    if (cmp)
        qsort_r(buffer, buflen, sizeof (struct dirent *), wrap_cmp, cmp);
    *retbuf = buffer;
    pthread_setcancelstate(cs, 0);
    return buflen;

error_free:
    err = errno;
    for (size_t i = 0; i < buflen; i++)
        free(buffer[i]);
    free(buffer);
    closedir(d);
    errno = err;
error:
    pthread_setcancelstate(cs, 0);
    return -1;
}
