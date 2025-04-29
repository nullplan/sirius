#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

static const struct resolvconf defconf = {
    .ns[0].addr = "\0\0\0\0\0\0\0\0\0\0\xff\xff\x7f\0\0\1",
    .ns[0].af = AF_INET,
    .nns = 1,
    .ndots = 1,
    .timeout = 5,
    .attempts = 3,
};

hidden void __get_resolvconf(struct resolvconf *c)
{
    *c = defconf;
}
