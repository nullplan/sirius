#include <stdio.h>
#include <string.h>

char *ctermid(char *s)
{
    return memcpy(s, "/dev/tty", 9);
}
