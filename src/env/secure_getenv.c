#include <stdlib.h>
#include "libc.h"

char *secure_getenv(const char *name)
{
    return __elevated? 0 : getenv(name);
}
