#ifndef ERRNO_H
#define ERRNO_H

#include "../../include/errno.h"
#include <pthread.h>
#undef errno
#define errno __pthread_self()->errno_val
#endif
