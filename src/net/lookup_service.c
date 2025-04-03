#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>

hidden int __lookup_service(const char *name, int flags, int type, struct service svc[static MAXSERVICE])
{
    int cnt = 0;
    unsigned long port = 0;
    char *endp = "";

    if (name)
        port = strtoul(name, &endp, 10);

    if (!*endp) {
        if (port >= 65536ul) return EAI_SERVICE;
        if (type != SOCK_DGRAM) {
            svc[cnt].port = port;
            svc[cnt].type = SOCK_STREAM;
            cnt++;
        }
        if (type != SOCK_STREAM) {
            svc[cnt].port = port;
            svc[cnt].type = SOCK_DGRAM;
            cnt++;
        }
        return cnt;
    }

    if (flags & AI_NUMERICSERV) return EAI_SERVICE;

    size_t namelen = strlen(name);
    char line[LINE_MAX + 1];
    FILE *f = fopen("/etc/services", "re");
    if (!f) return EAI_SERVICE;

    line[0] = ' ';
    while (cnt < MAXSERVICE && fgets(line + 1, sizeof line - 1, f)) {
        char *p = strchr(line, '#');
        if (p) {
            *p++ = ' ';
            *p = 0;
        }

        for (p = strstr(line, name); p; p = strstr(p+1, name))
            if (isspace(p[-1]) && isspace(p[namelen]))
                break;

        if (p) {
            for (p = line + 1; *p && !isspace(*p); p++);
            port = strtoul(p, &endp, 10);
            if (port < 65536ul) {
                if (type != SOCK_DGRAM && !memcmp(endp, "/tcp", 4)) {
                    svc[cnt].port = port;
                    svc[cnt].type = SOCK_STREAM;
                    cnt++;
                }
                if (type != SOCK_STREAM && !memcmp(endp, "/udp", 4)) {
                    svc[cnt].port = port;
                    svc[cnt].type = SOCK_DGRAM;
                    cnt++;
                }
            }
        }

    }
    fclose(f);
    return cnt;
}
