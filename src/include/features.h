#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define hidden __attribute__((visibility("hidden")))
#define weak __attribute__((weak))

#define weak_alias(new, old) extern __typeof__(old) new __attribute__((__weak__, alias(#old)))
#endif
