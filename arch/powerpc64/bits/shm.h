typedef unsigned long shmatt_t;
struct shmid_ds {
    struct ipc_perm shm_perm;
    time_t shm_atime;
    time_t shm_dtime;
    time_t shm_ctime;
    size_t shm_segsz;
    pid_t shm_cpid;
    pid_t shm_lpid;
    shmatt_t shm_nattch;
    unsigned long __unused[2];
};
