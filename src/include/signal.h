#ifndef SIGNAL_H
#define SIGNAL_H

#include "../../include/signal.h"
#include <pthread.h>

extern hidden struct lock __abort_lock;
hidden int __libc_sigaction(int, const struct sigaction *restrict, struct sigaction *restrict);
hidden int __sigaction(int, const struct sigaction *restrict, struct sigaction *restrict);
hidden void __init_sigs(void);
hidden int __block_usr_sigs(sigset_t *);
hidden int __block_all_sigs(sigset_t *);
hidden int __restore_sigs(const sigset_t *);

#endif
