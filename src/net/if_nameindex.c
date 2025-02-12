#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "netlink.h"

static int prepare(struct nlmsghdr *nlh, size_t sz, void *c)
{
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
    struct rtgenmsg *rtg = NLMSG_DATA(nlh);
    rtg->rtgen_family = 0;
    return NLMSG_LENGTH(sizeof *rtg);
}

struct ifhash {
    struct if_nameindex *index;
    size_t n;
    size_t capa;
};

static int handle(const struct nlmsghdr *nlh, void *c)
{
    struct ifhash *hash = c;
    if (nlh->nlmsg_type != RTM_NEWLINK || nlh->nlmsg_len < NLMSG_LENGTH(sizeof (struct ifinfomsg)))
        return 0;
    struct ifinfomsg *ifi = NLMSG_DATA(nlh);
    int rtalen = nlh->nlmsg_len - NLMSG_LENGTH(sizeof (struct ifinfomsg));
    for (struct rtattr *rta = (void *)(ifi + 1); RTA_OK(rta, rtalen); rta = RTA_NEXT(rta, rtalen))
    {
        if (rta->rta_type == IFLA_IFNAME) {
            if (rta->rta_len > IF_NAMESIZE)
                return 0;
            if (hash->n == hash->capa) {
                size_t attempt = hash->capa * 2 + 1;
                void *p = realloc(hash->index, attempt * sizeof (struct if_nameindex));
                if (!p) {
                    errno = ENOBUFS;
                    return -1;
                }
                hash->index = p;
                hash->capa = attempt;
            }
            struct if_nameindex *p = hash->index + hash->n;
            p->if_index = ifi->ifi_index;
            memcpy(p->__buf, RTA_DATA(rta), rta->rta_len);
            break;
        }
    }
    return 0;
}

struct if_nameindex *if_nameindex(void)
{
    struct ifhash hash = {0};
    if (__netlink_transact(prepare, 0, handle, &hash) == -1) {
        free(hash.index);
        return 0;
    }

    if (hash.n == hash.capa) {
        void *p = realloc(hash.index, (hash.capa + 1) * sizeof (struct if_nameindex));
        if (!p) {
            free(hash.index);
            errno = ENOBUFS;
            return 0;
        }
        hash.index = p;
    }

    for (size_t i = 0; i < hash.n; i++)
        hash.index[i].if_name = hash.index[i].__buf;
    hash.index[hash.n].if_index = 0;
    hash.index[hash.n].if_name = 0;
    return hash.index;
}

