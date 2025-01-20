#ifndef SYS_STAT_H
#define SYS_STAT_H

#include "../../../include/sys/stat.h"

extern hidden int __fstat(int, struct stat *);
extern hidden int __fstatat(int, const char *restrict, struct stat *restrict, int);

#endif
