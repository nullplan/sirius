#ifndef __NET_IF_H
#define __NET_IF_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#define IF_NAMESIZE 16

struct if_nameindex {
    unsigned if_index;
    char *if_name;
    char __buf[IF_NAMESIZE];
};

void if_freenameindex(struct if_nameindex *);
struct if_nameindex *if_nameindex(void);

char *if_indextoname(unsigned, char *);
unsigned if_nametoindex(const char *);
#ifdef __cplusplus
}
#endif
#endif
