#include <net/if.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "netlink.h"

static int prepare(struct nlmsghdr *nlh, size_t size, void *ctx)
{
    unsigned idx = *(unsigned *)ctx;
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST;
    struct ifinfomsg *ifi = NLMSG_DATA(nlh);
    memset(ifi, 0, sizeof *ifi);
    ifi->ifi_index = idx;
    return sizeof (struct nlmsghdr) + sizeof (struct ifinfomsg);
}

struct handlectx {
    unsigned idx;
    char *buf;
};

static int handle(const struct nlmsghdr *nlh, void *c)
{
    struct handlectx *ctx = c;
    const struct ifinfomsg *ifi = NLMSG_DATA(nlh);
    if (nlh->nlmsg_type != RTM_NEWLINK
            || nlh->nlmsg_len < sizeof (struct nlmsghdr) + sizeof (struct ifinfomsg)
            || ifi->ifi_index != ctx->idx)
        return 0;

    int rtalen = nlh->nlmsg_len - sizeof (struct nlmsghdr) - sizeof (struct ifinfomsg);
    for (const struct rtattr *rta = (void *)(ifi + 1); RTA_OK(rta, rtalen); rta = RTA_NEXT(rta, rtalen))
    {
        if (rta->rta_type == IFLA_IFNAME) {
            assert(rta->rta_len <= IF_NAMESIZE);
            memcpy(ctx->buf, RTA_DATA(rta), rta->rta_len);
            return 1;
        }
    }
    return 0;
}

char *if_indextoname(unsigned idx, char *buf)
{
    char *rv = 0;
    if (idx && __netlink_transact(prepare, &idx, handle, &(struct handlectx){idx, buf}) == 1)
        rv = buf;
    else
        errno = ENXIO;
    return rv;
}
