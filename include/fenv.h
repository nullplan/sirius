#ifndef __FENV_H
#define __FENV_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bits/fenv.h>

typedef struct { int __rm; } femode_t;
#define FE_DFL_MODE ((const femode_t *)-1)
#define FE_DFL_ENV  ((const fenv_t *)-1)

int feclearexcept(int);
int fegetexceptflag(fexcept_t *, int);
int feraiseexcept(int);
int fesetexcept(int);
int fesetexceptflag(const fexcept_t *, int);
int fetestexceptflag(const fexcept_t *, int);
int fetestexcept(int);
int fegetround(void);
int fesetround(int);
int fegetmode(femode_t *);
int fesetmode(const femode_t *);
int fegetenv(fenv_t *);
int feholdexcept(fenv_t *);
int fesetenv(const fenv_t *);
int feupdateenv(const fenv_t *);

#ifdef __cplusplus
}
#endif
#endif
