#ifndef __SYS_UN_H
#define __SYS_UN_H

#define __NEED_sa_family_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr_un {
    sa_family_t sun_family;
    char sun_path[108];
};

#ifdef __cplusplus
}
#endif
#endif
