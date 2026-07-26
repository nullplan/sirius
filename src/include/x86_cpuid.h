#ifndef X86_CPUID_H
#define X86_CPUID_H

#include <features.h>
#include <stdint.h>

extern hidden uintptr_t __cpuid;
#define X86_FEAT_FMA 1
#define X86_FEAT_FMA4 2
#endif
