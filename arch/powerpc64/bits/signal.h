#define MINSIGSTKSZ     8192
#define SIGSTKSZ        32768

typedef struct {
    unsigned long __unused[4];
    int __NSG(signal);
    int __pad0;
    unsigned long __NSG(handler);
    unsigned long __NSG(oldmask);
    void *__pad1;
    unsigned long __NSG(gp_regs)[48];
    double __NSG(fp_regs)[33];
    void *__NSG(v_regs);
    long __reserve[101];
} mcontext_t;

typedef struct __uc {
    unsigned long uc_flags;
    struct __uc *uc_link;
    stack_t uc_stack;
    sigset_t uc_sigmask;
    mcontext_t uc_mcontext;
} ucontext_t;
