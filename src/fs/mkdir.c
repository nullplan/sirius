#include <sys/stat.h>

int mkdir(const char *name, mode_t m)
{
    return mkdirat(-100, name, m);
}
