#include <unistd.h>
#include <string.h>
#include <errno.h>

int getlogin_r(char *buf, size_t len)
{
    char *login = getlogin();
    if (!login) return ENXIO;
    size_t loglen = strlen(login);
    if (loglen >= len) return ERANGE;
    memcpy(buf, login, loglen + 1);
    return 0;
}
