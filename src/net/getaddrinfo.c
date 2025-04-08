#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int getaddrinfo(const char *restrict nodename, const char *restrict servname, const struct addrinfo *restrict hints, struct addrinfo **restrict result)
{
    struct service service[MAXSERVICE];
    struct address address[MAXADDR];
    char canonname[256];

    int type = 0;
    int family = AF_UNSPEC;
    int flags = 0;

    if (hints) {
        type = hints->ai_socktype;
        family = hints->ai_family;
        flags = hints->ai_flags;

        if (type != 0 && type != SOCK_STREAM && type != SOCK_DGRAM)
            return EAI_SOCKTYPE;
        if (type == 0 && hints->ai_protocol != 0)
            return EAI_SOCKTYPE;
        if (type == SOCK_STREAM && hints->ai_protocol != 0 && hints->ai_protocol != IPPROTO_TCP)
            return EAI_SOCKTYPE;
        if (type == SOCK_DGRAM && hints->ai_protocol != 0 && hints->ai_protocol != IPPROTO_UDP)
            return EAI_SOCKTYPE;

        if (family != AF_UNSPEC && family != AF_INET && family != AF_INET6)
            return EAI_FAMILY;

        if (flags >= 128)
            return EAI_BADFLAGS;
    }
    int nserv = __lookup_service(servname, flags, type, service);
    if (nserv < 0) return nserv;
    if (nserv == 0) return EAI_SERVICE;

    int naddr = __lookup_name(nodename, flags, family, address, canonname);
    if (naddr < 0) return naddr;
    if (naddr == 0) return EAI_NONAME;

    size_t canonlen = flags & AI_CANONNAME? strlen(canonname) + 1 : 0;
    struct addrinfo *buf = malloc(naddr * nserv * (sizeof (struct addrinfo) + sizeof (struct sockaddr_in6)) + canonlen);
    if (!buf) return EAI_MEMORY;

    struct addrinfo *first = buf;
    *result = first;
    struct sockaddr_in6 *sa = (void *)(buf + naddr * nserv);
    char *canonretbuf = canonlen? (void *)(sa + naddr * nserv) : 0;
    if (canonlen) memcpy(canonretbuf, canonname, canonlen);

    for (int i = 0; i < naddr; i++)
        for (int j = 0; j < nserv; j++)
        {
            buf->ai_flags = 0;
            buf->ai_family = address[i].af;
            buf->ai_socktype = service[j].type;
            buf->ai_protocol = service[j].type == SOCK_STREAM? IPPROTO_TCP : IPPROTO_UDP;
            buf->__first = 0;
            buf->ai_addr = (void *)sa;
            buf->ai_canonname = canonretbuf;
            buf->ai_next = buf + 1;
            buf->ai_addrlen = address[i].af == AF_INET? sizeof (struct sockaddr_in) : sizeof (struct sockaddr_in6);
            if (address[i].af == AF_INET) {
                struct sockaddr_in *sin = (void *)sa;
                memset(sin, 0, sizeof *sin);
                sin->sin_family = AF_INET;
                sin->sin_port = htons(service[j].port);
                memcpy(&sin->sin_addr, address[i].addr + 12, 4);
            } else {
                memset(sa, 0, sizeof *sa);
                sa->sin6_family = AF_INET6;
                sa->sin6_port = htons(service[j].port);
                memcpy(&sa->sin6_addr, address[i].addr, 16);
                sa->sin6_scope_id = address[i].scope_id;
            }
            buf++;
            sa++;
        }
    buf[-1].ai_next = 0;
    first->__first = 1;
    return 0;
}
