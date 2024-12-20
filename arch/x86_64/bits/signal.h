#define SA_RESTORER     0x04000000
#define MINSIGSTKSZ     4096
#define SIGSTKSZ        8192

struct __fpx_sw_bytes {
    unsigned __NSG(magic1);
    unsigned __NSG(extended_size);
    unsigned long long __NSG(xfeatures);
    unsigned __NSG(xstate_size);
    unsigned __NSG(padding)[7];
};

struct __fpstate {
    unsigned short __NSG(cwd), __NSG(swd), __NSG(twd), __NSG(fop);
    unsigned long long __NSG(rip), __NSG(rdp);
    unsigned __NSG(mxcsr);
    unsigned __NSG(mxcsr_mask);
    unsigned __NSG(st_space)[32];
    unsigned __NSG(xmm_space)[64];
    unsigned __NSG(reserved2)[12];
    struct __fpx_sw_bytes __NSG(sw_reserved);
};

typedef struct {
    unsigned long long __NSG(r8);
    unsigned long long __NSG(r9);
    unsigned long long __NSG(r10);
    unsigned long long __NSG(r11);
    unsigned long long __NSG(r12);
    unsigned long long __NSG(r13);
    unsigned long long __NSG(r14);
    unsigned long long __NSG(r15);
    unsigned long long __NSG(rdi);
    unsigned long long __NSG(rsi);
    unsigned long long __NSG(rbp);
    unsigned long long __NSG(rbx);
    unsigned long long __NSG(rdx);
    unsigned long long __NSG(rax);
    unsigned long long __NSG(rcx);
    unsigned long long __NSG(rsp);
    unsigned long long __NSG(rip);
    unsigned long long __NSG(eflags);
    unsigned short __NSG(cs), __NSG(gs), __NSG(fs), __NSG(ss);
    unsigned long long __NSG(err);
    unsigned long long __NSG(trapno);
    unsigned long long __NSG(oldmask);
    unsigned long long __NSG(cr2);
    struct __fpstate *__NSG(fpstate);
    unsigned long long __reserved[8];
} mcontext_t;

typedef struct __uc {
    unsigned long uc_flags;
    struct __uc *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
} ucontext_t;
