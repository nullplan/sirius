#include <net/if.h>
#include <string.h>
#include "netlink.h"

extern hidden int __netlink_transact(int (*)(struct nlmsghdr *, size_t, void *), void *, int (*)(const struct nlmsghdr *, void *), void *);
struct prepctx {
    const char *name;
    size_t len;
};

static int prepare(struct nlmsghdr *nlh, size_t sz, void *c)
{
    struct prepctx *ctx = c;
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST;

    struct ifinfomsg *ifi = NLMSG_DATA(nlh);
    memset(ifi, 0, sizeof *ifi);

    struct rtattr *rta = (void *)(ifi + 1);
    rta->rta_type = IFLA_IFNAME;
    rta->rta_len = ctx->len;
    memcpy(rta + 1, ctx->name, ctx->len);
    return sizeof (struct nlmsghdr) + sizeof (struct ifinfomsg) + sizeof (struct rtattr) + ctx->len;
}

static int handle(const struct nlmsghdr *nlh, void *c)
{
    if (nlh->nlmsg_type != RTM_NEWLINK || nlh->nlmsg_len < NLMSG_LENGTH(sizeof (struct ifinfomsg))) return 0;
    struct ifinfomsg *ifi = NLMSG_DATA(nlh);
    *(unsigned *)c = ifi->ifi_index;
    return 1;
}

unsigned if_nametoindex(const char *n)
{
    size_t len = strnlen(n, IF_NAMESIZE);
    if (len - 1u >= IF_NAMESIZE - 1) return 0;
    unsigned rv = 0;
    __netlink_transact(prepare, &(struct prepctx){n, len + 1}, handle, &rv);
    return rv;
}
