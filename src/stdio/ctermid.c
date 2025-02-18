#include <stdio.h>
#include <string.h>

char *ctermid(char *s)
{
    return strcpy(s, "/dev/tty");
}
