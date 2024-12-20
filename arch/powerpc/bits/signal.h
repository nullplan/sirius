#define SA_RESTORER     0x04000000
#define MINSIGSTKSZ     2048
#define SIGSTKSZ        8192

typedef struct {
    unsigned long __NSG(mc_gregs)[48];
    double __NSG(mc_fregs)[33];
    unsigned long __pad[3];
    unsigned long __NSG(mc_vrregs)[33][4];
} mcontext_t;
typedef struct __uc {
    unsigned long uc_flags;
    struct __uc *uc_link;
    stack_t uc_stack;
    int __pad[8];
    sigset_t uc_sigmask;
    int __pad2[3];
    mcontext_t uc_mcontext;
} ucontext_t;
