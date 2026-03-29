#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    struct sockaddr_storage sa;
    memset(&sa, 0, sizeof sa);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <addr> <port>\n");
        return 1;
    }

    struct sockaddr_in *sin = (void *)&sa;
    struct sockaddr_in6 *sin6 = (void *)&sa;
    if (inet_pton(AF_INET, argv[1], &sin->sin_addr) > 0) {
        sin->sin_family = AF_INET;
        sin->sin_port = htons(strtol(argv[2], 0, 10));
    } else if (inet_pton(AF_INET6, argv[1], &sin6->sin6_addr) > 0) {
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(strtol(argv[2], 0, 10));
    } else {
        fprintf(stderr, "Address format not recognized\n");
        return 1;
    }

    char nodename[256];
    char servname[32];
    int rv = getnameinfo((void *)&sa, sizeof sa, nodename, sizeof nodename, servname, sizeof servname, 0);
    if (rv < 0) {
        fprintf(stderr, "getnameinfo failed: %s\n", gai_strerror(rv));
        return 1;
    }

    printf("Node: %s\nService: %s\n", nodename, servname);
    return 0;
}
