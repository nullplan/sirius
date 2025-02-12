#ifndef NETLINK_H
#define NETLINK_H

/* this is how to interact with Linux-specific kernel features: Copy the
 * relevant definitions into your own code and userspace-ify them as necessary.
 */
#include <stdint.h>
#include <features.h>
#include <stddef.h>


#define NETLINK_ROUTE   0

struct nlmsghdr {
    uint32_t nlmsg_len;
    uint16_t nlmsg_type;
    uint16_t nlmsg_flags;
    uint32_t nlmsg_seq;
    uint32_t nlmsg_pid;
};

#define NLM_F_REQUEST   0x0001
#define NLM_F_MULTI     0x0002
#define NLM_F_ACK       0x0004
#define NLM_F_ECHO      0x0008
#define NLM_F_DUMP_INTR 0x0010
#define NLM_F_DUMP_FILTERED     0x0020
#define NLM_F_ROOT      0x0100
#define NLM_F_MATCH     0x0200
#define NLM_F_ATOMIC    0x0400
#define NLM_F_DUMP      0x0300
#define NLM_F_REPLACE   0x0100
#define NLM_F_EXCL      0x0200
#define NLM_F_CREATE    0x0400
#define NLM_F_APPEND    0x0800
#define NLM_F_NONREC    0x0100
#define NLM_F_BULK      0x0200
#define NLM_F_CAPPED    0x0100
#define NLM_F_ACK_TLVS  0x0200

#define NLMSG_ALIGN(len)        (((len)+3) & -4U)
#define NLMSG_HDRLEN            16
#define NLMSG_LENGTH(len)       ((len) + 16)
#define NLMSG_SPACE(len)        (NLMSG_ALIGN(len) + 16)
#define NLMSG_DATA(nlh)         ((void *)((char *)(nlh) + 16))
#define NLMSG_NEXT(nlh, len)    ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
        (struct nlmsghdr *)((char *)(nlh) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_OK(nlh,len)       ((len) >= 16 && (nlh)->nlmsg_len >= 16 && (nlh)->nlmsg_len <= (len))
#define NLMSG_PAYLOAD(nlh,len)  (nlh->nlmsg_len - NLMSG_SPACE((len)))

#define NLMSG_NOOP              0x1
#define NLMSG_ERROR             0x2
#define NLMSG_DONE              0x3
#define NLMSG_OVERRUN           0x4

struct nlmsgerr {
    int error;
    struct nlmsghdr msg;
};

#define RTM_NEWLINK             16
#define RTM_DELLINK             17
#define RTM_GETLINK             18
#define RTM_SETLINK             19

struct rtattr {
    uint16_t rta_len;
    uint16_t rta_type;
};

#define RTA_ALIGN(len)          (((len)+3) & -4U)
#define RTA_OK(rta,len)         ((len) >= 4 && (rta)->rta_len >= 4 && (rta)->rta_len <= (len))
#define RTA_NEXT(rta,len)       ((len) -= RTA_ALIGN((rta)->rta_len), \
        (struct rtattr *)((char *)(rta) + RTA_ALIGN((rta)->rta_len)))
#define RTA_DATA(ret)           ((void *)((char *)(rta) + 4))

struct rtgenmsg {
    unsigned char rtgen_family;
};

struct ifinfomsg {
    unsigned char ifi_family;
    unsigned char __ifi_pad;
    unsigned short ifi_type;
    int ifi_index;
    unsigned ifi_flags;
    unsigned ifi_change;
};

#define IFLA_IFNAME             3

extern hidden int __netlink_transact(int (*)(struct nlmsghdr *, size_t, void *), void *, int (*)(const struct nlmsghdr *, void *), void *);

#endif
