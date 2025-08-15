#ifndef __NETINET_IN_H
#define __NETINET_IN_H

#define __NEED_uint8_t
#define __NEED_uint16_t
#define __NEED_uint32_t
#define __NEED_sa_family_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct in_addr {
    in_addr_t s_addr;
};

struct sockaddr_in {
    sa_family_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
    unsigned char __pad[8];
};

struct in6_addr {
    uint8_t s6_addr[16];
};

struct sockaddr_in6 {
    sa_family_t sin6_family;
    in_port_t sin6_port;
    uint32_t sin6_flowinfo;
    struct in6_addr sin6_addr;
    uint32_t sin6_scope_id;
};

extern const struct in6_addr in6addr_any;
#define IN6ADDR_ANY_INIT {{0}}
extern const struct in6_addr in6addr_loopback;
#define IN6ADDR_LOOPBACK_INIT {{[15] = 1}}

struct ipv6_mreq {
    struct in6_addr ipv6mr_multiaddr;
    unsigned ipv6mr_interface;
};

enum {
  IPPROTO_IP = 0,
  IPPROTO_ICMP = 1,
  IPPROTO_IGMP = 2,
  IPPROTO_IPIP = 4,
  IPPROTO_TCP = 6,
  IPPROTO_EGP = 8,
  IPPROTO_PUP = 12,
  IPPROTO_UDP = 17,
  IPPROTO_IDP = 22,
  IPPROTO_TP = 29,
  IPPROTO_DCCP = 33,
  IPPROTO_IPV6 = 41,
  IPPROTO_RSVP = 46,
  IPPROTO_GRE = 47,
  IPPROTO_ESP = 50,
  IPPROTO_AH = 51,
  IPPROTO_MTP = 92,
  IPPROTO_BEETPH = 94,
  IPPROTO_ENCAP = 98,
  IPPROTO_PIM = 103,
  IPPROTO_COMP = 108,
  IPPROTO_L2TP = 115,
  IPPROTO_SCTP = 132,
  IPPROTO_UDPLITE = 136,
  IPPROTO_MPLS = 137,
  IPPROTO_ETHERNET = 143,
  IPPROTO_RAW = 255,
  IPPROTO_SMC = 256,
  IPPROTO_MPTCP = 262,
};

#define INADDR_ANY  0
#define INADDR_BROADCAST    0xffffffffU
#define INET_ADDRSTRLEN     16
#define INET6_ADDRSTRLEN    46

#ifdef __LITTLE_ENDIAN
#include <bswap.h>
#define htons(x)    __bswap16(x)
#define ntohs(x)    __bswap16(x)
#define htonl(x)    __bswap32(x)
#define ntohl(x)    __bswap32(x)
#else
#define htons(x)    ((uint16_t)((x) * 1))
#define ntohs(x)    ((uint16_t)((x) * 1))
#define htonl(x)    ((uint32_t)((x) * 1))
#define ntohl(x)    ((uint32_t)((x) * 1))
#endif

#define IPV6_ADDRFORM           1
#define IPV6_2292PKTINFO        2
#define IPV6_2292HOPOPTS        3
#define IPV6_2292DSTOPTS        4
#define IPV6_2292RTHDR          5
#define IPV6_2292PKTOPTIONS     6
#define IPV6_CHECKSUM           7
#define IPV6_2292HOPLIMIT       8
#define IPV6_NEXTHOP            9
#define IPV6_AUTHHDR            10
#define IPV6_FLOWINFO           11
#define IPV6_UNICAST_HOPS       16
#define IPV6_MULTICAST_IF       17
#define IPV6_MULTICAST_HOPS     18
#define IPV6_MULTICAST_LOOP     19
#define IPV6_ADD_MEMBERSHIP     20
#define IPV6_DROP_MEMBERSHIP    21
#define IPV6_ROUTER_ALERT       22
#define IPV6_MTU_DISCOVER       23
#define IPV6_MTU                24
#define IPV6_RECVERR            25
#define IPV6_V6ONLY             26
#define IPV6_JOIN_ANYCAST       27
#define IPV6_LEAVE_ANYCAST      28
#define IPV6_MULTICAST_ALL      29
#define IPV6_ROUTER_ALERT_ISOLATE       30
#define IPV6_RECVERR_RFC4884    31
#define IPV6_FLOWLABEL_MGR      32
#define IPV6_FLOWINFO_SEND      33
#define IPV6_IPSEC_POLICY       34
#define IPV6_XFRM_POLICY        35
#define IPV6_HDRINCL            36
#define IPV6_RECVPKTINFO        49
#define IPV6_PKTINFO            50
#define IPV6_RECVHOPLIMIT       51
#define IPV6_HOPLIMIT           52
#define IPV6_RECVHOPOPTS        53
#define IPV6_HOPOPTS            54
#define IPV6_RTHDRDSTOPTS       55
#define IPV6_RECVRTHDR          56
#define IPV6_RTHDR              57
#define IPV6_RECVDSTOPTS        58
#define IPV6_DSTOPTS            59
#define IPV6_RECVPATHMTU        60
#define IPV6_PATHMTU            61
#define IPV6_DONTFRAG           62
#define IPV6_RECVTCLASS         66
#define IPV6_TCLASS             67
#define IPV6_AUTOFLOWLABEL      70
#define IPV6_ADDR_PREFERENCES   72
#define IPV6_MINHOPCOUNT                73
#define IPV6_ORIGDSTADDR        74
#define IPV6_RECVORIGDSTADDR    IPV6_ORIGDSTADDR
#define IPV6_TRANSPARENT        75
#define IPV6_UNICAST_IF         76
#define IPV6_RECVFRAGSIZE       77
#define IPV6_FREEBIND           78
#define IPV6_JOIN_GROUP     IPV6_ADD_MEMBERSHIP
#define IPV6_LEAVE_GROUP    IPV6_DROP_MEMBERSHIP
#define IPV6_PMTUDISC_DONT              0
#define IPV6_PMTUDISC_WANT              1
#define IPV6_PMTUDISC_DO                2
#define IPV6_PMTUDISC_PROBE             3
#define IPV6_PMTUDISC_INTERFACE         4
#define IPV6_PMTUDISC_OMIT              5
#define IPV6_PREFER_SRC_TMP             0x0001
#define IPV6_PREFER_SRC_PUBLIC          0x0002
#define IPV6_PREFER_SRC_PUBTMP_DEFAULT  0x0100
#define IPV6_PREFER_SRC_COA             0x0004
#define IPV6_PREFER_SRC_HOME            0x0400
#define IPV6_PREFER_SRC_CGA             0x0008
#define IPV6_PREFER_SRC_NONCGA          0x0800

#define __EQ12(x, a, b, c, d, e, f, g, h, i, j, k, l) ((x)[0] == a \
        && (x)[1] == b \
        && (x)[2] == c \
        && (x)[3] == d \
        && (x)[4] == e \
        && (x)[5] == f \
        && (x)[6] == g \
        && (x)[7] == h \
        && (x)[8] == i \
        && (x)[9] == j \
        && (x)[10] == k \
        && (x)[11] == l)
#define __EQ16(x, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) \
    (__EQ12(x, a, b, c, d, e, f, g, h, i, j, k, l) \
    && (x)[12] == m \
    && (x)[13] == n \
    && (x)[14] == o \
    && (x)[15] == p)

#define IN6_IS_ADDR_UNSPECIFIED(x)  __EQ16((x)->s6_addr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define IN6_IS_ADDR_LOOPBACK(x)     __EQ16((x)->s6_addr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#define IN6_IS_ADDR_MULTICAST(x)    ((x)->s6_addr[0] == 0xff)
#define IN6_IS_ADDR_LINKLOCAL(x)    ((x)->s6_addr[0] == 0xfe && ((x)->s6_addr[1] & 0xc0) == 0x80)
#define IN6_IS_ADDR_SITELOCAL(x)    0
#define IN6_IS_ADDR_V4MAPPED(x)     __EQ12((x)->s6_addr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff)
#define IN6_IS_ADDR_V4COMPAT(x)     0
#define IN6_IS_ADDR_MC_NODELOCAL(x) ((x)->s6_addr[0] == 0xff && ((x)->s6_addr & 0x0f) == 1)
#define IN6_IS_ADDR_MC_LINKLOCAL(x) ((x)->s6_addr[0] == 0xff && ((x)->s6_addr & 0x0f) == 2)
#define IN6_IS_ADDR_MC_SITELOCAL(x) ((x)->s6_addr[0] == 0xff && ((x)->s6_addr & 0x0f) == 5)
#define IN6_IS_ADDR_MC_ORGLOCAL(x)  ((x)->s6_addr[0] == 0xff && ((x)->s6_addr & 0x0f) == 8)
#define IN6_IS_ADDR_MC_GLOBAL(x)    ((x)->s6_addr[0] == 0xff && ((x)->s6_addr & 0x0f) == 14)

#ifdef __cplusplus
}
#endif
#endif
