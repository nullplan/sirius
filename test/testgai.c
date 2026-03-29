#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <service>\n");
        return 1;
    }

    struct addrinfo *res, *iter, hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
    int rv = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (rv) {
        fprintf(stderr, "getaddrinfo(`%s', `%s') = %s\n", argv[1], argv[2], gai_strerror(rv));
        return 0;
    }
    for (iter = res; iter; iter = iter->ai_next)
    {
        printf("Result with cname = %s, af = %d, socktype = %d, protocol = %d\n", iter->ai_canonname, iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        struct sockaddr_in *sin = (void *)iter->ai_addr;
        struct sockaddr_in6 *sin6 = (void *)iter->ai_addr;
        char addrbuf[INET6_ADDRSTRLEN];
        const char *addr = inet_ntop(iter->ai_family, iter->ai_family == AF_INET? (void *)&sin->sin_addr : &sin6->sin6_addr, addrbuf, sizeof addrbuf);
        int port = iter->ai_family == AF_INET? ntohs(sin->sin_port) : ntohs(sin6->sin6_port);
        printf("Addr [%s]:%d\n", addr, port);
    }
    freeaddrinfo(res);
    return 0;
}
