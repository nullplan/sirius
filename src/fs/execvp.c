#include <unistd.h>
#include <stdlib.h>

int execvp(const char *file, char *const args[])
{
    return __execvpe(file, args, __environ);
}
