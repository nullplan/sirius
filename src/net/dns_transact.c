#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>

static uint32_t mtime(void)
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) && errno == ENOSYS)
        clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000 + now.tv_nsec / 1000000;
}

static void cleanup(void *p)
{
    for (struct pollfd *pfd = p; pfd->fd >= -1; pfd++)
        if (pfd->fd >= 0) close(pfd->fd);
}

static int start_tcp(struct pollfd *pfd, const unsigned char *q, size_t qlen, const struct address *addr)
{
    int sk = socket(addr->af, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sk < 0) return -1;
    union {
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
    } uaddr;
    socklen_t slen = addr->af == AF_INET? sizeof (struct sockaddr_in) : sizeof (struct sockaddr_in6);
    unsigned char lenbuf[2] = {qlen >> 8, qlen};
    struct msghdr mh = {.msg_name = (void *)&uaddr,
        .msg_namelen = slen,
        .msg_iovlen = 2,
        .msg_iov = (struct iovec[]){
            {.iov_base = lenbuf, .iov_len = 2},
            {.iov_base = (void *)q, .iov_len = qlen},
        }
    };

    memset(&uaddr, 0, slen);
    if (addr->af == AF_INET) {
        uaddr.sin.sin_family = AF_INET;
        uaddr.sin.sin_port = htons(53);
        memcpy(&uaddr.sin.sin_addr, addr->addr + 12, 4);
    } else {
        uaddr.sin6.sin6_family = AF_INET6;
        uaddr.sin6.sin6_port = htons(53);
        memcpy(&uaddr.sin6.sin6_addr, addr->addr, 16);
        uaddr.sin6.sin6_scope_id = addr->scope_id;
    }
    pfd->fd = sk;
    pfd->events = POLLOUT;
    ssize_t snd = sendmsg(sk, &mh, MSG_FASTOPEN | MSG_NOSIGNAL);
    if (snd == qlen + 2) {
        pfd->events = POLLIN;
        return 0;
    } 
    if (snd >= 0) return snd;
    if (errno == EINPROGRESS) return 0;

    if (!connect(sk, (struct sockaddr *)&uaddr, slen) || errno == EINPROGRESS)
        return 0;
    close(sk);
    return -1;
}

hidden int __dns_transact(unsigned char *const *query, const size_t *querylen, int nquery, const struct resolvconf *conf, unsigned char *const *result, size_t *resultlen)
{
    /* workflow:
     * Check configured nameservers:
     *  - only IPv6 -> use IPv6 socket
     *  - only IPv4 -> use IPv4 socket
     *  - mixed -> use IPv6 socket with v4 mapping
     * Open necessary socket. If IPv6 socket fails and v4 makes sense, try to fall back.
     * Send all queries to all servers separately on UDP.
     * Read responses:
     *  - filter all responses from different hosts
     *  - filter all responses with wrong query ID
     *  - if rcode == servfail, retry with that server once
     *  - if rcode == NXDOMAIN, accept result
     *  - if rcode == 0 and TC, attempt TCP with the given server for the given query.
     *  - otherwise reject (i.e. ignore) response
     *
     * TCP workflow:
     *  TCP is only attempted with a specific server and a specific query.
     *  Make socket asynchronous
     *  Attempt TCP fastopen
     *          - on failure: attempt connect()
     *          - on EINPROGRESS: assume connect() is in progress, but no data sent
     *          - if return is nonnegative, assume connect() succeeded and the given amount of data sent.
     *  Normal async workflow:
     *          - attempt connect()
     *          - wait for socket to be writable
     *          - get socket error to know if connect succeeded or how exactly it failed.
     *          - if successful, proceed to send(), wait for writable
     *          - if everything was sent, switch to waiting for readable
     *          - do not resend queries after timeout (it is TCP, if send() says it got there, then it got there).
     *          - if timeout occurs, just abort everything.
     */
    memset(resultlen, 0, nquery * sizeof (size_t)); /* resultlen[i] > 0 indicates an answer is available. */
    struct pollfd pfd[nquery + 2];
    int num_ipv6 = 0;
    int num_ipv4 = 0;
    for (int i = 0; i < nquery; i++)
        pfd[i+1].fd = -1;
    pfd[nquery + 1].fd = -2; /* terminating entry */

    for (int i = 0; i < conf->nns; i++)
        if (conf->ns[i].af == AF_INET)
            num_ipv4++;
        else
            num_ipv6++;

    int af = AF_INET6;
    if (num_ipv6)
    {
        pfd[0].fd = socket(AF_INET6, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
        if (pfd[0].fd >= 0
                && ((num_ipv4 && setsockopt(pfd[0].fd, IPPROTO_IPV6, IPV6_V6ONLY, &(int){0}, sizeof (int)))
                    || bind(pfd[0].fd, (struct sockaddr *)&(struct sockaddr_in6){.sin6_family = AF_INET6}, sizeof (struct sockaddr_in6))))
        {
            close(pfd[0].fd);
            pfd[0].fd = -1;
        }
    }

    if (pfd[0].fd == -1)
    {
        if (!num_ipv4) return -1;
        pfd[0].fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
        if (pfd[0].fd >= 0 && bind(pfd[0].fd, (struct sockaddr *)&(struct sockaddr_in){.sin_family = AF_INET}, sizeof (struct sockaddr_in)))
        {
            close(pfd[0].fd);
            pfd[0].fd = -1;
        }
        af = AF_INET;
    }

    if (pfd[0].fd == -1) return -1;
    pfd[0].events = POLLIN;

    pthread_cleanup_push(cleanup, pfd);
    int timeout_interval = conf->timeout * 1000;
    int retry_interval = timeout_interval / conf->attempts;
    uint32_t now = mtime();
    uint32_t lastsend = now - retry_interval;
    uint32_t start = now;
    int nfd = 1;
    union {
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
    } uaddr;
    socklen_t slen;
    int servfail_retry[conf->nns];

    for (; now - start < timeout_interval; now = mtime())
    {
        {
            int i;
            for (i = 0; i < nquery; i++)
                if (!resultlen[i])
                    break;
            if (i == nquery) break; /* we are so done! */
        }
        if (now - lastsend >= retry_interval)
        {
            if (af == AF_INET) {
                memset(&uaddr.sin, 0, sizeof (struct sockaddr_in));
                uaddr.sin.sin_family = AF_INET;
                uaddr.sin.sin_port = htons(53);
                slen = sizeof (struct sockaddr_in);
            } else {
                memset(&uaddr.sin6, 0, sizeof (struct sockaddr_in6));
                uaddr.sin6.sin6_family = AF_INET6;
                uaddr.sin6.sin6_port = htons(53);
                slen = sizeof (struct sockaddr_in6);
            }

            for (int i = 0; i < conf->nns; i++)
            {
                if (af == AF_INET6 || conf->ns[i].af == AF_INET) {
                    if (af == AF_INET6) {
                        memcpy(&uaddr.sin6.sin6_addr, conf->ns[i].addr, 16);
                        uaddr.sin6.sin6_scope_id = conf->ns[i].scope_id;
                    } else {
                        memcpy(&uaddr.sin.sin_addr, conf->ns[i].addr + 12, 4);
                    }
                    for (int j = 0; j < nquery; j++)
                        if (!resultlen[j])
                            sendto(pfd[0].fd, query[j], querylen[j], 0, (struct sockaddr *)&uaddr, slen);
                }
                servfail_retry[i] = 1;
            }
            lastsend = now;
        }
        if (poll(pfd, nfd, lastsend - retry_interval - now) <= 0) continue;

        /* UDP needs special handling that TCP really doesn't need */
        if (pfd[0].revents & POLLIN) {
            int bufnum;
            for (bufnum = 0; bufnum < nquery; bufnum++)
                if (!resultlen[bufnum])
                    break;
            /* we have to find a result buffer; the case of everything found was excluded already */
            slen = af == AF_INET? sizeof (struct sockaddr) : sizeof (struct sockaddr_in);
            ssize_t rcv = recvfrom(pfd[0].fd, result[bufnum], 512, 0, (struct sockaddr *)&uaddr, &slen);
            /* ignore too-small results and errors on UDP */
            if (rcv < 12) continue;

            /* ignore packets from unknown server */
            int server;
            for (server = 0; server < conf->nns; server++)
                if ((af == AF_INET6 && !memcmp(&uaddr.sin6.sin6_addr, conf->ns[server].addr, 16))
                        || (af == AF_INET && !memcmp(&uaddr.sin.sin_addr, conf->ns[server].addr + 12, 4)))
                    break;
            if (server == conf->nns) continue;

            if ((af == AF_INET6 && ntohs(uaddr.sin6.sin6_port) != 53)
                    || (af == AF_INET && ntohs(uaddr.sin.sin_port) != 53))
                continue;

            /* match XID */
            int querynum;
            for (querynum = 0; querynum < nquery; querynum++)
                if (!memcmp(result[bufnum], query[querynum], 2))
                    break;
            if (querynum == nquery) continue;

            /* if query was already answered or TCP is in progress, ignore UDP packet */
            if (resultlen[querynum] || pfd[querynum + 1].fd >= 0) continue;
            if (querynum != bufnum) memcpy(result[querynum], result[bufnum], rcv);

            int rcode = result[querynum][3] & 15;
            if (rcode == 2 && servfail_retry[server]) {
                servfail_retry[server] = 0;
                sendto(pfd[0].fd, query[querynum], querylen[querynum], 0, (struct sockaddr *)&uaddr, slen);
                continue;
            } else if (rcode != 3 && rcode != 0)
                continue;
            if (result[querynum][2] & 2) {
                start_tcp(pfd + querynum + 1, query[querynum], querylen[querynum]);
                continue;
            }

            resultlen[querynum] = rcv;
        }
    }

    pthread_cleanup_pop(1);
    return 0;
}
