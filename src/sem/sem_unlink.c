#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

int sem_unlink(const char *name)
{
    while (*name == '/') name++;
    size_t len = strnlen(name, NAME_MAX + 1);
    if (len > NAME_MAX) {
        errno = ENAMETOOLONG;
        return -1;
    }
    if (!len || strchr(name, '/')) {
        errno = EINVAL;
        return -1;
    }
    char fullnamebuf[sizeof "/dev/shm/" + NAME_MAX];
    memcpy(fullnamebuf, "/dev/shm/", 9);
    memcpy(fullnamebuf + 9, name, len + 1);
    return unlink(fullnamebuf);
}
