#ifndef LIBC_H
#define LIBC_H

#include <features.h>
#include <stddef.h>
#include <limits.h>
struct __pthread;
extern hidden long __sysinfo;
extern hidden int __elevated;
extern hidden unsigned long __default_stacksize;
extern hidden size_t __page_size;
extern hidden void __run_constructors(void);
extern hidden void __run_destructors(void);
extern hidden void __stdio_exit(void);
extern hidden void __run_atexit_funcs(void);
extern hidden struct __pthread *__init_from_phdrs(const void *, size_t, size_t);
extern hidden size_t __next_canary(void);
extern hidden void __init_canary(const void *);

#ifdef PAGESIZE
#define PAGE_SIZE PAGESIZE
#else
#define PAGE_SIZE __page_size
#endif
#endif
