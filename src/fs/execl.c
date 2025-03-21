#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

int execl(const char *file, const char *a0, ...)
{
    va_list ap;
    size_t argc = 1;
    va_start(ap, a0);
    while (va_arg(ap, const char *))
        argc++;
    va_end(ap);
    const char *args[argc + 1];
    args[0] = a0;
    va_start(ap, a0);
    for (size_t i = 1; i < argc; i++)
        args[i] = va_arg(ap, const char *);
    va_end(ap);
    args[argc] = 0;
    return execve(file, (char **)args, __environ);
}
