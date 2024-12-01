#ifndef __CTYPE_H
#define __CTYPE_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int);
int isalpha(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int toupper(int);
int tolower(int);

#ifdef __cplusplus
}
#endif
#endif
