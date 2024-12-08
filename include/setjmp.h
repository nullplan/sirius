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

#ifdef __cplusplus
}
#endif
#endif
