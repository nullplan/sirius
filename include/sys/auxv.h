#ifndef __SYS_AUXV_H
#define __SYS_AUXV_H

#include <elf.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned long getauxval(unsigned long);

#ifdef __cplusplus
}
#endif
#endif
