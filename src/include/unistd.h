#ifndef UNISTD_H
#define UNISTD_H

#include "../../include/unistd.h"
extern hidden off_t __lseek(int, off_t, int);
extern hidden void __randname(char *);
extern hidden int __execvpe(const char *, char *const [], char *const []);
extern hidden int __setxid(int, int, int, int);
#endif
