struct semid_ds {
    struct ipc_perm sem_perm;
    time_t  sem_otime;
    time_t  sem_ctime;
    unsigned short sem_nsems;
    unsigned short __pad[3];
    unsigned long __unused[2];
};
