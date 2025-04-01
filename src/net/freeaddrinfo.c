#include <netdb.h>
#include <stdlib.h>

void freeaddrinfo(struct addrinfo *ai)
{
    if (ai->__first) free(ai);
}
