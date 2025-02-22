#include <sys/stat.h>

int mkfifo(const char *name, mode_t m)
{
    return mkfifoat(-100, name, m);
}
