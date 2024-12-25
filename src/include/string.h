#ifndef STRING_H
#define STRING_H

#include "../../include/string.h"

hidden size_t __stridx(const char *, int);
hidden size_t __strridx(const char *, int);
hidden char *__strtok_r(char *restrict, const char *restrict, char **restrict);
hidden char *__stpcpy(char *restrict, const char *restrict);
hidden char *__stpncpy(char *restrict, const char *restrict, size_t);
#endif
