#ifndef __NETDB_H
#define __NETDB_H

#define __NEED_uint32_t
#define __NEED_socklen_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr;

struct hostent {
    char *h_name;
    char **h_aliases;
    int h_addrtype;
    int h_length;
    char **h_addr_list;
};

struct netent {
    char *n_name;
    char **n_aliases;
    int n_addrtype;
    uint32_t n_net;
};

struct protoent {
    char *p_name;
    char **p_aliases;
    int p_proto;
};

struct servent {
    char *s_name;
    char **s_aliases;
    int s_port;
    char *s_proto;
};

#define IPPORT_RESERVED     1024

struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    int __first;
    struct sockaddr *ai_addr;
    char *ai_canonname;
    struct addrinfo *ai_next;
};

#define AI_PASSIVE      1
#define AI_CANONNAME    2
#define AI_NUMERICHOST  4
#define AI_NUMERICSERV  8
#define AI_V4MAPPED     16
#define AI_ALL          32
#define AI_ADDRCONFIG   64

#define NI_NOFQDN       1
#define NI_NUMERICHOST  2
#define NI_NAMEREQD     4
#define NI_NUMERICSERV  8
#define NI_NUMERICSCOPE 16
#define NI_DGRAM        32

#define EAI_AGAIN       (-1)
#define EAI_BADFLAGS    (-2)
#define EAI_FAIL        (-3)
#define EAI_FAMILY      (-4)
#define EAI_MEMORY      (-5)
#define EAI_NONAME      (-6)
#define EAI_SERVICE     (-7)
#define EAI_SOCKTYPE    (-8)
#define EAI_SYSTEM      (-9)
#define EAI_OVERFLOW    (-10)

void sethostent(int);
struct hostent *gethostent(void);
void endhostent(void);
void setnetent(int);
struct netent *getnetent(void);
void endnetent(void);
void setprotoent(int);
struct protoent *getprotoent(void);
void endprotoent(void);
void setservent(int);
struct servent *getservent(void);
void endservent(void);

struct netent *getnetbyaddr(uint32_t, int);
struct netent *getnetbyname(const char *);
struct protoent *getprotobynumber(int);
struct protoent *getprotobyname(const char *);
struct servent *getservbyport(int, const char *);
struct servent *getservbyname(const char *, const char *);

int getaddrinfo(const char *__restrict, const char *__restrict, const struct addrinfo *__restrict, struct addrinfo **__restrict);
int getnameinfo(const struct sockaddr *__restrict, socklen_t, char *__restrict, socklen_t, char *__restrict, socklen_t, int);
void freeaddrinfo(struct addrinfo *);
const char *gai_strerror(int);

#ifdef __cplusplus
}
#endif
#endif
