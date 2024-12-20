struct semid_ds {
    struct ipc_perm sem_perm;
    time_t  sem_otime;
    unsigned long long __pad1;
    time_t  sem_ctime;
    unsigned long long __pad2;
    unsigned short sem_nsems;
    unsigned short __pad[3];
    unsigned long long __unused[2];
};
