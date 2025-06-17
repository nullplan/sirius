#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s pass salt\n", argv[0]);
        return 1;
    }

    printf("crypt(`%s', `%s') = `%s'\n", argv[1], argv[2], crypt(argv[1], argv[2]));
    return 0;
}
