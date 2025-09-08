#define _GNU_SOURCE
#include <errno.h>
#include "libc.h"

char *__progname;
char *__progname_short;

weak_alias(program_invocation_name, __progname);
weak_alias(program_invocation_short_name, __progname_short);

hidden void __init_progname(char *pn)
{
    __progname = __progname_short = pn;
    for (; *pn; pn++)
        if (*pn == '/')
            __progname_short = pn + 1;
}
