#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include "futex.h"
#include "cpu.h"

static void cleanup(void *p) {
    once_flag *flg = p;
    if (a_swap(&flg->__ctrl, 0) == 3)
        __futex_wake(&flg->__ctrl, 1, 1);
}

void call_once(once_flag *flg, void (*fct)(void))
{
    if (flg->__ctrl == 2) {
        a_barrier();
        return;
    }

    for (;;) {
        switch (a_cas(&flg->__ctrl, 0, 1)) {
            case 0:
                pthread_cleanup_push(cleanup, flg);
                fct();
                pthread_cleanup_pop(0);
                if (a_swap(&flg->__ctrl, 2) == 3)
                    __futex_wake(&flg->__ctrl, 1, INT_MAX);
                break;

            case 1:
                if (a_cas(&flg->__ctrl, 1, 3) != 1) continue;
                /* fall through */
            case 3:
                __futex_wait(&flg->__ctrl, 1, 3);
                break;

            case 2:
                return;

        }
    }
}
