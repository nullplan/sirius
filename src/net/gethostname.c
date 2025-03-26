#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>

int gethostname(char *buf, size_t len)
{
    struct utsname un;
    if (uname(&un)) return -1;
    strncpy(buf, un.nodename, len);
    return 0;
}
