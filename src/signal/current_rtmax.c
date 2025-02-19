#include <signal.h>

int __current_rtmax(void) { return _NSIG - 1; }
