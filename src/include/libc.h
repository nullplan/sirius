#ifndef LIBC_H
#define LIBC_H

#include <features.h>
extern hidden void __run_constructors(void);
extern hidden void __run_destructors(void);
extern hidden void __stdio_exit(void);
extern hidden void __run_atexit_funcs(void);
#endif
