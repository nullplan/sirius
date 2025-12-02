typedef unsigned long shmatt_t;
/* we might be on BE. To be independent, always calculate the time. */
#define __ARCH_SHM_CALC_TIMES
struct shmid_ds {
    struct ipc_perm shm_perm;
    size_t shm_segsz;
    unsigned long __shm_atime;
    unsigned long __shm_atime_high;
    unsigned long __shm_dtime;
    unsigned long __shm_dtime_high;
    unsigned long __shm_ctime;
    unsigned long __shm_ctime_high;
    pid_t shm_cpid;
    pid_t shm_lpid;
    unsigned long shm_nattch;
    unsigned long __unused[2];
    time_t shm_atime;
    time_t shm_dtime;
    time_t shm_ctime;
};
