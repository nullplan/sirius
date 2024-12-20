#ifndef __SYS_TIME_H
#define __SYS_TIME_H

#include <features.h>
#include <sys/select.h>

#ifdef __cplusplus
extern "C" {
#endif

int utimes(const char *, const struct timeval [2]);

#ifdef __cplusplus
}
#endif
#endif
