#include <pthread.h>

int pthread_getcpuclockid(pthread_t t, clockid_t *clk)
{
    /* ~tid << 3 | 6 = 8 * (-tid - 1) + 6 = -8 * tid - 2 */
    /* not taking the killlock since it wouldn't help.
     * Someone fix this API, please!
     */
    *clk = -8 * t->tid - 2;
    return 0;
}
