#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

hidden int __lookup_ipliteral(const char *name, int fam, struct address *addr)
{
    char buf[INET6_ADDRSTRLEN];
    const char *suffix = 0;
    size_t len = __stridx(name, '%');
    if (len >= sizeof buf) return 0;
    if (name[len])
    {
        memcpy(buf, name, len);
        buf[len] = 0;
        suffix = name + len + 1;
        name = buf;
    }

    if (fam != AF_INET6 && inet_pton(AF_INET, name, addr->addr + 12) == 1)
    {
        memcpy(addr->addr, "\0\0\0\0\0\0\0\0\0\0\377\377", 12);
        addr->af = AF_INET;
        return 1;
    }

    if (fam != AF_INET && inet_pton(AF_INET6, name, addr->addr) == 1)
    {
        unsigned long idx = 0;
        if (suffix) {
            char *endp;
            idx = strtoul(suffix, &endp, 10);
            if (*endp) idx = if_nametoindex(suffix);
        }
        addr->af = AF_INET6;
        addr->scope_id = idx;
        return 1;
    }

    return 0;
}
