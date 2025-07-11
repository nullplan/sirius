#include <sys/ipc.h>
#include <sys/stat.h>

key_t ftok(const char *fn, int id)
{
    struct stat st;
    if (stat(fn, &st)) return -1;
    return (st.st_ino & 0xffff) | (st.st_dev & 0xff) << 16 | (id & 0xffU) << 24;
}
