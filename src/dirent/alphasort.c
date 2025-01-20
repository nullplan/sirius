#include <dirent.h>
#include <string.h>

int alphasort(const struct dirent **da, const struct dirent **db)
{
    return strcoll((*da)->d_name, (*db)->d_name);
}
