typedef unsigned long msgqnum_t;
typedef unsigned long msglen_t;
#define __ARCH_MSG_CALC_TIMES
struct msqid_ds {
    struct ipc_perm msg_perm;
    unsigned long __msg_stime;
    unsigned long __msg_stime_high;
    unsigned long __msg_rtime;
    unsigned long __msg_rtime_high;
    unsigned long __msg_ctime;
    unsigned long __msg_ctime_high;
    unsigned long __NSG(msg_cbytes);
    msgqnum_t msg_qnum;
    msglen_t msg_qbytes;
    pid_t msg_lspid;
    pid_t msg_lrpid;
    time_t msg_stime;
    time_t msg_rtime;
    time_t msg_ctime;
};
