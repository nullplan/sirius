#include <unistd.h>

int symlink(const char *ofn, const char *nfn)
{
    return symlinkat(ofn, -100, nfn);
}
