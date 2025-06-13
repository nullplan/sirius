#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *optarg;
int opterr = 1, optind = 1, optopt;
static size_t optchar = 1;
int getopt(int argc, char *const argv[], const char *optstr)
{
    if (*optstr == '+')
        optstr++;
    int do_msg = opterr && *optstr != ':';

    /* if optind is set to zero, the behavior is unspecified!
     * I choose to specify it as a state reset.
     */
    if (!optind) optind = optchar = 1;

    /* but optind = 0 is not standard, so ensure optchar = 1 when we return -1 */
    if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == 0)
    {
        optchar = 1;
        return -1;
    }

    if (argv[optind][1] == '-' && argv[optind][2] == 0)
    {
        optind++;
        optchar = 1;
        return -1;
    }

    int c = argv[optind][optchar];

    const char *p = 0;
    if (c != ':')
        p = strchr(optstr, c);

    if (!p || p[1] != ':') {
        /* presumed single-option flag */
        optchar++;
        if (!argv[optind][optchar]) {
            optind++;
            optchar = 1;
        }
        if (!p) {
            /* unknown option */
            if (do_msg)
                fprintf(stderr, "%s: Unknown option `%c'.\n", argv[0], c);
            optopt = c;
            return '?';
        }
    } else if (!argv[optind][optchar + 1]) {
        /* option with argument in next arg */
        optarg = argv[optind + 1];
        optind += 2;
        optchar = 1;
        if (optind > argc) {
            if (do_msg)
                fprintf(stderr, "%s: Missing argument for option `%c'.\n", argv[0], c);
            optopt = c;
            return *optstr == ':'? ':' : '?';
        }
    } else {
        /* option with argument being the remainder of the arg */
        optarg = argv[optind] + optchar + 1;
        optind++;
        optchar = 1;
    }
    
    return c;
}
