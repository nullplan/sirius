#include <sys/mman.h>
#include <fcntl.h>

int shm_open(const char *name, int oflg, mode_t mode)
{
    char shmname[SHM_NAMELEN];
    if (__shm_getname(shmname, name)) return -1;
    return __openat(AT_FDCWD, shmname, oflg, mode);
}
