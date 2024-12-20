#define __ARCH_SEM_CALC_TIMES
struct semid_ds {
    struct ipc_perm sem_perm;
    unsigned long __sem_otime_high;
    unsigned long __sem_otime;
    unsigned long __sem_ctime_high;
    unsigned long __sem_ctime;
#ifdef _LITTLE_ENDIAN
    unsigned short sem_nsems;
    unsigned short __pad;
#else
    unsigned short __pad;
    unsigned short sem_nsems;
#endif
    time_t sem_otime;
    time_t sem_ctime;
};
