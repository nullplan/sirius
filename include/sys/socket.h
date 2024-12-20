#ifndef __SYS_SOCKET_H
#define __SYS_SOCKET_H

#define __NEED_socklen_t
#define __NEED_sa_family_t
#define __NEED_struct_iovec
#define __NEED_size_t
#define __NEED_ssize_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14 - sizeof (long)];
    long __align;
};

struct sockaddr_storage {
    sa_family_t ss_family;
    char ss_data[126 - sizeof (long)];
    long __align;
};

struct msghdr {
    void           *msg_name;
    socklen_t       msg_namelen;
    struct iovec   *msg_iov;
    __IF_64_BE(int __pad1;)
    int             msg_iovlen;
    __IF_64_LE(int __pad1;)
    void           *msg_control;
    __IF_64_BE(int __pad2;)
    socklen_t       msg_controllen;
    __IF_64_LE(int __pad2;)
    int             msg_flags;
};

struct cmsghdr {
    __IF_64_BE(int __pad1;)
    socklen_t cmsg_len;
    __IF_64_LE(int __pad1;)
    int cmsg_level;
    int cmsg_type;
};

#define SCM_RIGHTS      1
#define SCM_CREDENTIALS 2
#define SCM_SECURITY    3
#define SCM_PIDFD       4

struct linger {
    int l_onoff;
    int l_linger;
};

#define __CMSG_ALIGN(x)     (((x) + sizeof (long) - 1) & (-sizeof (long)))
#define CMSG_DATA(cm)       ((unsigned char *)((cm)+1))
#define CMSG_FIRSTHDR(mh)   ((mh)->msg_controllen < sizeof (struct cmsghdr)? 0 : (struct cmsghdr *)((mh)->msg_control))
#define CMSG_NXTHDR(mh, cm) (!(cm)? CMSG_FIRSTHDR(mh) : \
        (char *)(cm) - (char *)((mh)->msg_control) + __CMSG_ALIGN((cm)->cmsg_len) + sizeof (struct cmsghdr) <= (mh)->msg_controllen? \
            (struct cmsghdr *)((char *)(cm) + __CMSG_ALIGN((cm)->cmsg_len)) : 0)
#define CMSG_LEN(x)         ((x) + sizeof (struct cmsghdr))
#define CMSG_SPACE(x)       __CMSG_ALIGN(CMSG_LEN(x))

#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3
#define SOCK_RDM        4
#define SOCK_SEQPACKET  5
#define SOCK_DCCP       6
#define SOCK_PACKET     10
#define SOCK_NONBLOCK      0x00000800
#define SOCK_CLOEXEC       0x00080000

#define SOL_SOCKET      1

#include <bits/socket.h>

#define SO_DEBUG        1
#define SO_REUSEADDR    2
#define SO_TYPE         3
#define SO_ERROR        4
#define SO_DONTROUTE    5
#define SO_BROADCAST    6
#define SO_SNDBUF       7
#define SO_RCVBUF       8
#define SO_SNDBUFFORCE  32
#define SO_RCVBUFFORCE  33
#define SO_KEEPALIVE    9
#define SO_OOBINLINE    10
#define SO_NO_CHECK     11
#define SO_PRIORITY     12
#define SO_LINGER       13
#define SO_BSDCOMPAT    14
#define SO_REUSEPORT    15
#define SO_BINDTODEVICE 25
#define SO_ATTACH_FILTER        26
#define SO_DETACH_FILTER        27
#define SO_PEERNAME             28
#define SO_ACCEPTCONN           30
#define SO_PEERSEC              31
#define SO_PASSSEC              34
#define SO_MARK                 36
#define SO_PROTOCOL             38
#define SO_DOMAIN               39
#define SO_RXQ_OVFL             40
#define SO_WIFI_STATUS          41
#define SCM_WIFI_STATUS SO_WIFI_STATUS
#define SO_PEEK_OFF             42
#define SO_NOFCS                43
#define SO_LOCK_FILTER          44
#define SO_SELECT_ERR_QUEUE     45
#define SO_BUSY_POLL            46
#define SO_MAX_PACING_RATE      47
#define SO_BPF_EXTENSIONS       48
#define SO_INCOMING_CPU         49
#define SO_ATTACH_BPF           50
#define SO_DETACH_BPF           SO_DETACH_FILTER
#define SO_ATTACH_REUSEPORT_CBPF        51
#define SO_ATTACH_REUSEPORT_EBPF        52
#define SO_CNX_ADVICE           53
#define SCM_TIMESTAMPING_OPT_STATS      54
#define SO_MEMINFO              55
#define SO_INCOMING_NAPI_ID     56
#define SO_COOKIE               57
#define SCM_TIMESTAMPING_PKTINFO        58
#define SO_PEERGROUPS           59
#define SO_ZEROCOPY             60
#define SO_TXTIME               61
#define SCM_TXTIME              SO_TXTIME
#define SO_BINDTOIFINDEX        62
#define SO_TIMESTAMP_OLD        29
#define SO_TIMESTAMPNS_OLD      35
#define SO_TIMESTAMPING_OLD     37
#define SO_TIMESTAMP_NEW        63
#define SO_TIMESTAMPNS_NEW      64
#define SO_TIMESTAMPING_NEW     65
#define SO_RCVTIMEO_NEW         66
#define SO_SNDTIMEO_NEW         67
#define SO_DETACH_REUSEPORT_BPF 68
#define SO_PREFER_BUSY_POLL     69
#define SO_BUSY_POLL_BUDGET     70
#define SO_NETNS_COOKIE         71
#define SO_BUF_LOCK             72
#define SO_RESERVE_MEM          73
#define SO_TXREHASH             74
#define SO_RCVMARK              75
#define SO_PASSPIDFD            76
#define SO_PEERPIDFD            77
#define SO_DEVMEM_LINEAR        78
#define SCM_DEVMEM_LINEAR       SO_DEVMEM_LINEAR
#define SO_DEVMEM_DMABUF        79
#define SCM_DEVMEM_DMABUF       SO_DEVMEM_DMABUF
#define SO_DEVMEM_DONTNEED      80

#define SCM_TIMESTAMP           SO_TIMESTAMP
#define SCM_TIMESTAMPNS         SO_TIMESTAMPNS
#define SCM_TIMESTAMPING        SO_TIMESTAMPING

#define SOMAXCONN       4096

#define MSG_OOB             0x00000001
#define MSG_PEEK            0x00000002
#define MSG_DONTROUTE       0x00000004
#define MSG_TRYHARD         0x00000004
#define MSG_CTRUNC          0x00000008
#define MSG_PROBE           0x00000010
#define MSG_TRUNC           0x00000020
#define MSG_DONTWAIT        0x00000040
#define MSG_EOR             0x00000080
#define MSG_WAITALL         0x00000100
#define MSG_FIN             0x00000200
#define MSG_SYN             0x00000400
#define MSG_CONFIRM         0x00000800
#define MSG_RST             0x00001000
#define MSG_ERRQUEUE        0x00002000
#define MSG_NOSIGNAL        0x00004000
#define MSG_MORE            0x00008000
#define MSG_WAITFORONE      0x00010000
#define MSG_BATCH           0x00040000
#define MSG_EOF             MSG_FIN
#define MSG_NO_SHARED_FRAGS 0x00080000
#define MSG_SOCK_DEVMEM     0x02000000
#define MSG_ZEROCOPY        0x04000000
#define MSG_FASTOPEN        0x20000000
#define MSG_CMSG_CLOEXEC    0x40000000

#define AF_UNSPEC       0
#define AF_UNIX         1
#define AF_LOCAL        1
#define AF_INET         2
#define AF_AX25         3
#define AF_IPX          4
#define AF_APPLETALK    5
#define AF_NETROM       6
#define AF_BRIDGE       7
#define AF_ATMPVC       8
#define AF_X25          9
#define AF_INET6        10
#define AF_ROSE         11
#define AF_DECnet       12
#define AF_NETBEUI      13
#define AF_SECURITY     14
#define AF_KEY          15
#define AF_NETLINK      16
#define AF_ROUTE        AF_NETLINK
#define AF_PACKET       17
#define AF_ASH          18
#define AF_ECONET       19
#define AF_ATMSVC       20
#define AF_RDS          21
#define AF_SNA          22
#define AF_IRDA         23
#define AF_PPPOX        24
#define AF_WANPIPE      25
#define AF_LLC          26
#define AF_IB           27
#define AF_MPLS         28
#define AF_CAN          29
#define AF_TIPC         30
#define AF_BLUETOOTH    31
#define AF_IUCV         32
#define AF_RXRPC        33
#define AF_ISDN         34
#define AF_PHONET       35
#define AF_IEEE802154   36
#define AF_CAIF         37
#define AF_ALG          38
#define AF_NFC          39
#define AF_VSOCK        40
#define AF_KCM          41
#define AF_QIPCRTR      42
#define AF_SMC          43
#define AF_XDP          44
#define AF_MCTP         45

#define SHUT_RD     0
#define SHUT_WR     1
#define SHUT_RDWR   2

#ifdef _GNU_SOURCE
struct ucred {
    int pid;
    int uid;
    int gid;
};
#endif

int accept(int, struct sockaddr *__restrict, socklen_t *__restrict);
int accept4(int, struct sockaddr *__restrict, socklen_t *__restrict, int);
int bind(int, const struct sockaddr *, socklen_t);
int connect(int, const struct sockaddr *, socklen_t);
int getpeername(int, struct sockaddr *__restrict, socklen_t *__restrict);
int getsockname(int, struct sockaddr *__restrict, socklen_t *__restrict);
int getsockopt(int, int, int, void *__restrict, socklen_t *__restrict);
int listen(int, int);
ssize_t recv(int, void *, size_t, int);
ssize_t recvfrom(int, void *__restrict, size_t, int, struct sockaddr *__restrict, socklen_t *__restrict);
ssize_t recvmsg(int, struct msghdr *, int);
ssize_t send(int, const void *, size_t, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
ssize_t sendmsg(int, const struct msghdr *, int);
int setsockopt(int, int, int, const void *, socklen_t);
int shutdown(int, int);
int sockatmark(int);
int socket(int, int, int);
int socketpair(int, int, int, int [2]);

#ifdef __cplusplus
}
#endif
#endif
