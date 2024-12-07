#ifndef __SETJMP_H
#define __SETJMP_H

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
