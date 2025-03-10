#include <time.h>

int clock_getcpuclockid(pid_t pid, clockid_t *clk)
{
    /* *clk = ~pid << 3 | 2 = (-pid-1)*8 + 2 = -pid * 8 - 6 */
    *clk = -pid * 8 - 6;
    return 0;
}
