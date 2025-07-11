#include <sys/mman.h>
#include <unistd.h>

int shm_unlink(const char *name)
{
    char shmname[SHM_NAMELEN];
    if (__shm_getname(shmname, name)) return -1;
    return unlink(shmname);
}
