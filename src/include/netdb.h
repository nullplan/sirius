#ifndef NETDB_H
#define NETDB_H

#include "../../include/netdb.h"

#include <stddef.h>
#include <stdint.h>

/* getaddrinfo()/gethostbyname()/getservbyname() middleware */
struct service {
    uint16_t port;
    uint16_t type;
};

struct address {
    int af;
    unsigned char addr[16]; /* if IPv4, this is the v4mapped address */
    int scope_id;
    int sortkey;
};

#define MAXSERVICE      2       /* at most one TCP and one UDP service, right? */
#define MAXADDR         16      /* ought to be enough for anyone */
extern hidden int __lookup_service(const char *, int, int, struct service [static MAXSERVICE]);
extern hidden int __lookup_name(const char *, int, int, struct address [static MAXADDR], char *);
extern hidden int __lookup_ipliteral(const char *, int, struct address *);

/* backend stuff */
#define MAXNS           3
struct resolvconf {
    struct address ns[MAXNS];
    int nns;
    char search[256];
    int ndots;
    int timeout;
    int attempts;
};
/* always succeeds: If resolvconf cannot be opened/processed, it returns defaults. */
extern hidden void __get_resolvconf(struct resolvconf *);

#define DNS_RR_A        1
#define DNS_RR_CNAME    5
#define DNS_RR_PTR      12
#define DNS_RR_AAAA     28
#define DNS_MAXNAME     256 /* as per RFC 1035, maximum name length is 256 */
#define DNS_MAXQUERY    (DNS_MAXNAME + 17)
extern hidden int __dns_create_query(const char *, int, unsigned char *, size_t);

#define DNS_MAXRESULT   768     /* DNS response to AAAA query with maxed-out CNAME */
extern hidden int __dns_transact(unsigned char *const *, const size_t *, int, const struct resolvconf *, unsigned char *const *, size_t *);
extern hidden void __dns_process(const unsigned char *, size_t, int (*)(const unsigned char *, size_t, const unsigned char *, size_t, int, void *), void *);
extern hidden int __dn_expand(const unsigned char *, const unsigned char *, size_t, char *, size_t);


#endif
