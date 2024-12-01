#ifndef __SETJMP_H
#define __SETJMP_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bits/setjmp.h>

int setjmp(jmp_buf)
#ifdef __GNUC__
    __attribute__((returns_twice))
#endif
    ;
_Noreturn void longjmp(jmp_buf, int);

#ifdef __cplusplus
}
#endif
#endif
