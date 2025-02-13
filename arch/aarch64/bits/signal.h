#define SA_RESTORER     0x04000000
#define MINSIGSTKSZ     5120
#define SIGSTKSZ        16384

typedef struct {
    unsigned long long __NSG(fault_address);
    unsigned long long __NSG(regs)[31];
    unsigned long long __NSG(sp);
    unsigned long long __NSG(pc);
    unsigned long long __NSG(pstate);
    unsigned long long __pad;
    unsigned char __reserved[4096];
} mcontext_t;

typedef struct __uc {
    unsigned long uc_flags;
    struct __uc *uc_link;
    stack_t uc_stack;
    sigset_t uc_sigmask;
    unsigned long __pad;
    mcontext_t uc_mcontext;
} ucontext_t;
