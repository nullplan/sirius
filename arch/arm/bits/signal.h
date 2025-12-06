#define SA_RESTORER     0x04000000
#define MINSIGSTKSZ     2048
#define SIGSTKSZ        8192

typedef struct {
    unsigned long __NSG(trap_no);
    unsigned long __NSG(error_code);
    unsigned long __NSG(oldmask);
    unsigned long __NSG(arm_r0);
    unsigned long __NSG(arm_r1);
    unsigned long __NSG(arm_r2);
    unsigned long __NSG(arm_r3);
    unsigned long __NSG(arm_r4);
    unsigned long __NSG(arm_r5);
    unsigned long __NSG(arm_r6);
    unsigned long __NSG(arm_r7);
    unsigned long __NSG(arm_r8);
    unsigned long __NSG(arm_r9);
    unsigned long __NSG(arm_r10);
    unsigned long __NSG(arm_fp);
    unsigned long __NSG(arm_ip);
    unsigned long __NSG(arm_sp);
    unsigned long __NSG(arm_lr);
    unsigned long __NSG(arm_pc);
    unsigned long __NSG(arm_cpsw);
    unsigned long __NSG(fault_address);
} mcontext_t;

typedef struct __uc {
    unsigned long       uc_flags;
    struct __uc        *uc_link;
    stack_t             uc_stack;
    mcontext_t          uc_mcontext;
    sigset_t            uc_sigmask;
    _Alignas(8) unsigned long __NSG(uc_regspace)[128];
} ucontext_t;
