#include <stdio.h>
#include <math.h>

int main(void)
{
    for (int i = 0; i < 96; i++)
    {
        long double x = 0.25 + i * 0x1p-7;
        long double y = 1.0L/sqrtl(x);
        printf("%u,\n", (unsigned)(y * 0x1p6 + 0.5));
    }
}
