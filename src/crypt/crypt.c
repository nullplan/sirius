#include <unistd.h>
#include "crypt.h"

char *crypt(const char *pass, const char *salt)
{
    static char buf[CRYPT_LEN];
    return __crypt_r(pass, salt, buf);
}
