typedef unsigned long shmatt_t;
#define __ARCH_SHM_CALC_TIMES
struct shmid_ds {
    struct ipc_perm shm_perm;
    unsigned long __shm_atime_high;
    unsigned long __shm_atime;
    unsigned long __shm_dtime_high;
    unsigned long __shm_dtime;
    unsigned long __shm_ctime_high;
    unsigned long __shm_ctime;
    unsigned long __unused;
    size_t shm_segsz;
    pid_t shm_cpid;
    pid_t shm_lpid;
    shmatt_t shm_nattch;
    time_t shm_atime;
    time_t shm_dtime;
    time_t shm_ctime;
};
