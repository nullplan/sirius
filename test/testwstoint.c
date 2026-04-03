#include <wchar.h>
#include <inttypes.h>
#include <stdio.h>

struct testcase {
    const wchar_t *in;
    int base;
    uintmax_t out;
};

static const struct testcase cases[] = {
    {L"123", 0, 123},
    {L"0123", 0, 83},
    {L"0x123", 0, 291},
};

int main(void)
{
    for (size_t i = 0; i < sizeof cases / sizeof *cases; i++)
    {
        uintmax_t rv = wcstoumax(cases[i].in, 0, cases[i].base);
        if (rv != cases[i].out)
            fprintf(stderr, "Case %zu failed: expected %ju, got %ju\n", i + 1, cases[i].out, rv);
    }
    return 0;
}
