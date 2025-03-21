#include <unistd.h>
#include <stdarg.h>

int execle(const char *file, const char *a0, ...)
{
    va_list ap;
    size_t argc = 1;
    va_start(ap, a0);
    while (va_arg(ap, const char *))
        argc++;
    char **envp = va_arg(ap, char **);
    va_end(ap);
    const char *args[argc + 1];
    args[0] = a0;
    va_start(ap, a0);
    for (size_t i = 1; i < argc; i++)
        args[i] = va_arg(ap, const char *);
    va_end(ap);
    args[argc] = 0;
    return execve(file, (char **)args, envp);
}
