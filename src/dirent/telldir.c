#include <dirent.h>

long telldir(DIR *d)
{
    return d->tell;
}
