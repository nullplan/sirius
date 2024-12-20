#ifndef __SETJMP_H
#define __SETJMP_H

#define __STDC_VERSION_SETJMP_H__ 202311L

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bits/setjmp.h>

int setjmp(jmp_buf) __ret_twice;
_Noreturn void longjmp(jmp_buf, int);

#ifdef _POSIX_C_SOURCE
typedef struct {
    jmp_buf __jb;
    long __ss[128/sizeof (long)];
} sigjmp_buf[1];

int sigsetjmp(sigjmp_buf, int) __ret_twice;
_Noreturn void siglongjmp(sigjmp_buf, int);
#endif

#ifdef __cplusplus
}
#endif
#endif
