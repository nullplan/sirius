struct semid_ds {
    struct ipc_perm sem_perm;
    time_t sem_otime;
    time_t sem_ctime;
#ifdef _LITTLE_ENDIAN
    unsigned short sem_nsems;
    unsigned short __pad[3];
#else
    unsigned short __pad[3];
    unsigned short sem_nsems;
#endif
    unsigned long __unused[2];
};
