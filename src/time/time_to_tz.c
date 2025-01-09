#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <limits.h>
#include <stdint.h>
#include "syscall.h"
#include "libc.h"

static struct lock lock;
static char tzbuf[2][TZNAME_MAX];
static char *__tzname[2];
static int __daylight;
static long __timezone;
static long dstzone;
static const char utc[] = "UTC";
static const void *infofile;
static const size_t infosize;
static const uint64_t *transitions;
static const char *abbrevs;

weak_alias(tzname, __tzname);
weak_alias(daylight, __daylight);
weak_alias(timezone, __timezone);

static void do_tzset(void)
{
    static char varbuf[32];
    static char *oldtz = varbuf;
    static size_t oldtzsize = sizeof varbuf;

    char *tz = getenv("TZ");
    if (!tz) tz = "/etc/localtime";
    if (!*tz) tz = utc;
    if (!strcmp(tz, oldtz)) return;

    size_t tzlen = strlen(tz);
    if (tzlen >= oldtzsize) {
        /* yes, I know this leaks, but I'll avoid free() to avoid pulling it
         * into static linking where it might be unwanted.
         * Most applications never change the TZ ever. Most TZs fit inside of varbuf.
         * Even if not, this is only reallocated once the next higher power of two is reached.
         */
        size_t l = oldtzsize;
        while (l <= tzlen) l *= 2;
        char *p = malloc(l);
        if (!p) return; /* XXX: What do? */
        oldtz = p;
        oldtzsize = l;
    }
    memcpy(oldtz, tz, tzlen + 1);

    if (infofile) {
        __syscall(SYS_munmap, infofile, infosize);
        infofile = 0;
        infosize = 0;
    }
    if (!is_posix_form(tz)) {
        if (*tz == ':') tz++;
        if (is_tz_acceptable(tz)) {
            static const char *const searchpath[] = { "/usr/share/zoneinfo/", "/share/zoneinfo/", "/etc/zoneinfo/" };
            for (size_t i = 0; i < sizeof searchpath / sizeof *searchpath; i++)
            {
                size_t spl = strlen(searchpath[i]);
                char pathbuf[spl + tzlen + 1];
                memcpy(pathbuf, searchpath[i], spl);
                memcpy(pathbuf + spl, tz, tzlen + 1);
                int fd = __sys_open(pathbuf, O_RDONLY);
                if (fd != -1) {
                    struct stat st;
                    void *p;
                    if (!fstat(fd, &st) && st.st_size < PTRDIFF_MAX && (p = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) != MAP_FAILED) {
                        infofile = p;
                        infosize = st.st_size;
                    }
                    __syscall(SYS_close, fd);
                    if (infofile) break;
                }
            }
            if (infofile) {
            }
        }
    }
}

static void __tzset(void)
{
    __lock(&lock);
    do_tzset();
    __unlock(&lock);
}
weak_alias(tzset, __tzset);

/*
struct tz {
    int gmtoff;
    int isdst;
    int oppoff;
    const char *name;
    const char *oppname;
};
*/
extern hidden struct tz __time_to_tz(long long, int);
