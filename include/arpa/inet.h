#ifndef __ARPA_INET_H
#define __ARPA_INET_H

#include <netinet/in.h>
#define __NEED_socklen_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

const char *inet_ntop(int, const void *__restrict, char *__restrict, socklen_t);
int inet_pton(int, const char *__restrict, void *__restrict);

#ifdef __cplusplus
}
#endif
#endif
