#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <fcntl.h>
#include <unistd.h>

#define ATTEMPTS    10000
char *tmpnam(char *s)
{
    char name[] = "/tmp/tmpnam_XXXXXX";
    for (size_t i = 0; i < ATTEMPTS; i++) {
        __randname(name + sizeof name - 7);
#ifdef SYS_access
        int rv = __syscall(SYS_access, name, 0);
#else
        int rv = __syscall(SYS_faccessat, AT_FDCWD, name, 0);
#endif
        if (rv == -ENOENT)
            return memcpy(s, name, sizeof name);
    }
    errno = EEXIST;
    return 0;

}
