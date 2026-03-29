#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

static const struct resolvconf defconf = {
    .ns[0].addr = "\0\0\0\0\0\0\0\0\0\0\xff\xff\x7f\0\0\1",
    .ns[0].af = AF_INET,
    .nns = 1,
    .ndots = 1,
    .timeout = 5,
    .attempts = 3,
};

static int parse_resconf_file(struct resolvconf *c)
{
    char linebuf[LINE_MAX];
    FILE *f = fopen("/etc/resolv.conf", "r");
    if (!f) return 0;
    c->nns = 0;
    c->search[0] = 0;
    c->ndots = defconf.ndots;
    c->timeout = defconf.timeout;
    c->attempts = defconf.attempts;
    while (fgets(linebuf, sizeof linebuf, f))
    {
        char *p = strchr(linebuf, '#');
        if (p) {
            *p++ = ' ';
            *p = 0;
        }
        if (!strncmp(linebuf, "nameserver", 10)
                && isspace((unsigned char)linebuf[10])
                && c->nns < MAXNS) {
            for (p = linebuf + 11; isspace((unsigned char)*p); p++);
            size_t len = strlen(p);
            if (len > 0 && isspace((unsigned char)p[len-1]))
                p[len-1] = 0;
            if (__lookup_ipliteral(p, AF_UNSPEC, c->ns + c->nns) > 0)
                c->nns++;
        } else if ((!strncmp(linebuf, "search", 6) || !strncmp(linebuf, "domain", 6)) && isspace((unsigned char)linebuf[6])) {
            for (p = linebuf + 7; isspace((unsigned char)*p); p++);
            size_t len = strlen(p);
            while (len > 0 && isspace((unsigned char)p[len-1]))
                len--;
            if (len < sizeof c->search)
            {
                memcpy(c->search, p, len);
                c->search[len] = 0;
            }
        } else if (!strncmp(linebuf, "options", 7) && isspace((unsigned char)linebuf[7])) {
            p = strstr(linebuf, "ndots:");
            if (p) {
                c->ndots = strtol(p + 6, 0, 10);
                if (c->ndots > 15) c->ndots = 15;
            }
            p = strstr(linebuf, "timeout:");
            if (p) {
                c->timeout = strtol(p + 8, 0, 10);
                if (c->timeout > 30) c->timeout = 30;
            }
            p = strstr(linebuf, "attempts:");
            if (p) {
                c->attempts = strtol(p + 9, 0, 10);
                if (c->attempts > 30) c->attempts = 30;
            }
        }
    }
    fclose(f);
    return c->nns;
}

hidden void __get_resolvconf(struct resolvconf *c)
{
    if (!parse_resconf_file(c))
        *c = defconf;
}
