#define SA_RESTORER     0x04000000
#define MINSIGSTKSZ     4096
#define SIGSTKSZ        8192

struct __fpreg {
    unsigned short __NSG(significand)[4];
    unsigned short __NSG(exponent);
};

struct __fpxreg {
    unsigned short __NSG(significand)[4];
    unsigned short __NSG(exponent);
    unsigned short __NSG(padding)[3];
};

struct __xmmreg {
    unsigned __NSG(element)[4];
};

struct __fpx_sw_bytes {
    unsigned __NSG(magic1);
    unsigned __NSG(extended_size);
    unsigned long long __NSG(xfeatures);
    unsigned __NSG(xstate_size);
    unsigned __NSG(padding)[7];
};

struct __fpstate {
    unsigned __NSG(cw), __NSG(sw), __NSG(tag), __NSG(ipoff), __NSG(cssel), __NSG(dataoff), __NSG(datasel);
    struct __fpreg __NSG(_st)[8];
    unsigned short __NSG(status);
    unsigned short __NSG(magic);
    unsigned __NSG(_fxsr_env)[6];
    unsigned __NSG(mxcsr);
    unsigned __pad;
    struct __fpxreg __NSG(_fxsr_st)[8];
    struct __xmmreg __NSG(_xmm)[8];
    unsigned __pad1[44];
    struct __fpx_sw_bytes __NSG(sw_reserved);
};

typedef struct {
    unsigned short __NSG(gs), __gsh;
    unsigned short __NSG(fs), __fsh;
    unsigned short __NSG(es), __esh;
    unsigned short __NSG(ds), __dsh;
    unsigned __NSG(edi);
    unsigned __NSG(esi);
    unsigned __NSG(ebp);
    unsigned __NSG(esp);
    unsigned __NSG(ebx);
    unsigned __NSG(edx);
    unsigned __NSG(ecx);
    unsigned __NSG(eax);
    unsigned __NSG(trapno);
    unsigned __NSG(err);
    unsigned __NSG(eip);
    unsigned short __NSG(cs), __csh;
    unsigned __NSG(eflags);
    unsigned __NSG(esp_at_signal);
    unsigned short __NSG(ss), __ssh;
    struct __fpstate *__NSG(fpstate);
    unsigned __NSG(oldmask);
    unsigned __NSG(cr2);
} mcontext_t;

typedef struct __uc {
    unsigned long uc_flags;
    struct __uc *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
} ucontext_t;
