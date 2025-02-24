#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

hidden int __execvpe(const char *name, char *const *args, char *const *env)
{
    size_t len = strnlen(name, NAME_MAX + 1);
    if (len - 1 >= NAME_MAX || memchr(name, '/', len)) return execve(name, args, env);
    char *p = getenv("PATH");
    if (!p || !*p) p = "/bin:/usr/bin";
    int important_errno = 0;

    const char *a, *z;
    for (a = p; *a; a = z + !!*z)
    {
        z = a + __stridx(a, ':');
        char buf[z - a + len + 2];
        const char *fullname;
        if (a == z)
            fullname = name;
        else
        {
            fullname = buf;
            memcpy(buf, a, z - a);
            buf[z - a] = '/';
            memcpy(buf + (z - a) + 1, name, len + 1);
        }
        if (!faccessat(AT_FDCWD, fullname, X_OK, AT_EACCESS))
            return execve(fullname, args, env);
        if (errno != ENOENT && errno != ENOTDIR)
            important_errno = errno;
    }
    errno = important_errno? important_errno : ENOENT;
    return -1;
}
