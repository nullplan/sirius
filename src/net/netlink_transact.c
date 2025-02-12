#include "netlink.h"
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

hidden int __netlink_transact(int (*prepare)(struct nlmsghdr *, size_t, void *), void *prepctx, int (*handle)(const struct nlmsghdr *, void *), void *handlectx)
{
    int sk = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (sk == -1) return -1;

    union {
        char buf[8192];
        struct nlmsghdr nlh;
    } u;

    int sendlen = prepare(&u.nlh, sizeof u, prepctx);
    u.nlh.nlmsg_len = sendlen;
    u.nlh.nlmsg_seq = 0;
    u.nlh.nlmsg_pid = 0;

    if (sendlen & 15)
        memset(u.buf + sendlen, 0, (-sendlen) & 15);

    ssize_t rv = send(sk, &u, NLMSG_ALIGN(sendlen), 0);
    if (rv < sendlen) {
        rv = -1;
        goto out_close;
    }

    for (;;)
    {
        rv = recv(sk, &u, sizeof u, MSG_DONTWAIT);
        if (rv <= 0) goto out_close;

        for (struct nlmsghdr *nlh = &u.nlh; NLMSG_OK(nlh, rv); nlh = NLMSG_NEXT(nlh, rv))
        {
            if (nlh->nlmsg_type == NLMSG_ERROR) {
                struct nlmsgerr *err = NLMSG_DATA(nlh);
                rv = err->error;
                goto out_close;
            }
            int handle_res = handle(nlh, handlectx);
            if (handle_res) {
                rv = handle_res;
                goto out_close;
            }
            if (!(nlh->nlmsg_flags & NLM_F_MULTI))
                break;
        }
    }

out_close:
    close(sk);
    return rv;
}
