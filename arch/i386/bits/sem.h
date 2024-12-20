#define __ARCH_SEM_CALC_TIMES
struct semid_ds {
    struct ipc_perm sem_perm;
    unsigned long __sem_otime;
    unsigned long __sem_otime_high;
    unsigned long __sem_ctime;
    unsigned long __sem_ctime_high;
    unsigned short sem_nsems;
    time_t  sem_otime;
    time_t  sem_ctime;
};
