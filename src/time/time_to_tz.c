#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <limits.h>
#include <stdint.h>
#include <ctype.h>
#include "syscall.h"
#include "libc.h"

static struct lock lock;
static char tzbuf[2][TZNAME_MAX + 1];
char *__tzname[2];
int __daylight;
long __timezone;
static long dstzone;
static const char utc[] = "UTC";
static const void *infofile;
static size_t infosize;
static const unsigned char *transitions;
static const unsigned char *transtypes;
static const unsigned char *lttypes;
static const char *abbrevs;
static long long maxtime;
struct rule {
    enum { GREGORIAN, JULIAN, WEEKDAY } type;
    union {
        long day;
        unsigned char para[3];
    } u;
    long time;
};
static struct rule rules[2];

weak_alias(tzname, __tzname);
weak_alias(daylight, __daylight);
weak_alias(timezone, __timezone);

static const char *getname(const char *s, char *buf, size_t len)
{
    if (*s == '<') {
        const char *z = strchr(s, '>');
        if (z && z - s - 1 < len) {
            memcpy(buf, s + 1, z - s - 1);
            buf[z - s - 1] = 0;
            return z + 1;
        }
    } else {
        size_t spn = strspn(s, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        if (spn >= 3 && spn < len) {
            memcpy(buf, s, spn);
            buf[spn] = 0;
            return s + spn;
        }
    }
    return 0;
}

static const char *getint(const char *s, long *val)
{
    char *endp;
    *val = strtol(s, &endp, 10);
    return endp == s? 0 : endp;
}

static const char *getoff(const char *s, long *secs)
{
    long hours, minutes, seconds;
    minutes = seconds = 0;
    s = getint(s, &hours);
    if (!s) return 0;
    if (*s == ':') {
        s = getint(s + 1, &minutes);
        if (!s) return 0;
        if (*s == ':') {
            s = getint(s + 1, &seconds);
            if (!s) return 0;
        }
    }
    if (hours - LONG_MIN/3600 + 0ul >= ULONG_MAX/3600 || minutes + 0ul >= 60ul || seconds + 0ul >= 60ul)
        return 0;
    *secs = hours * 3600 + minutes * 60 + seconds;
    return s;
}

static const char *getrule(const char *s, struct rule *r)
{
    char *endp;
    if (isdigit(*s)) {
        r->type = GREGORIAN;
        r->u.day = strtol(s, &endp, 10);
    } else if (*s == 'J' && isdigit(s[1])) {
        r->type = JULIAN;
        r->u.day = strtol(s + 1, &endp, 10);
    } else if (*s == 'M'
            && s[1] - '1' < 9u
            && s[2] == '.'
            && s[3] - '1' < 5u
            && s[4] == '.'
            && s[5] - '0' < 7u)
    {
        r->type = WEEKDAY;
        r->u.para[0] = s[1] - '0';
        r->u.para[1] = s[3] - '0';
        r->u.para[2] = s[5] - '0';
        endp = (char *)s + 6;
    } else if (*s == 'M'
            && s[1] == '1'
            && s[2] - '0' < 3u
            && s[3] == '.'
            && s[4] - '1' < 5u
            && s[5] == '.'
            && s[6] - '0' < 7u)
    {
        r->type = WEEKDAY;
        r->u.para[0] = s[2] - '0' + 10;
        r->u.para[1] = s[4] - '0';
        r->u.para[2] = s[6] - '0';
        endp = (char *)s + 7;
    } else return 0;
    s = endp;
    r->time = 7200;
    if (*s == '/')
        s = getoff(s + 1, &r->time);
    return s;
}

static int is_posix_form(const char *tz)
{
    if (*tz == ':') return 0;
    char dummy[TZNAME_MAX + 1];
    tz = getname(tz, dummy, sizeof dummy);
    if (!tz || (*tz != '-' && *tz != '+' && !isdigit(*tz))) return 0;
    return 1;

}

static int is_tz_acceptable(const char *tz)
{
    return !__elevated || strcmp(tz, "/etc/localtime") == 0 || (tz[0] != '/' && memchr(tz, 0, NAME_MAX) && !strstr(tz, "../"));
}

static int parse_posix_form(const char *s)
{
    s = getname(s, tzbuf[0], TZNAME_MAX + 1);
    if (!s) return -1;
    s = getoff(s, &__timezone);
    if (!s) return -1;
    __tzname[0] = tzbuf[0];
    __daylight = 0;
    if (!*s) return 0;
    s = getname(s, tzbuf[1], TZNAME_MAX + 1);
    if (!s) return 0;   /* implementation definition: if no rule is given, dst is ignored (std is observed the whole year) */
    __tzname[1] = tzbuf[1];
    const char *p = getoff(s, &dstzone);
    if (!p) dstzone = __timezone - 3600;
    else s = p;
    if (*s != ',') return 0;
    s = getrule(s + 1, &rules[0]);
    if (!s || *s != ',') return 0;
    s = getrule(s + 1, &rules[1]);
    if (s)
        __daylight = 1;
    return 0;
}

static uint32_t read_i32(const unsigned char *x)
{
    return (x[0] + 0ul) << 24 | x[1] << 16 | x[2] << 8 | x[3];
}

static uint64_t read_i64(const unsigned char *x)
{
    return (read_i32(x) + 0ull) << 32 | read_i32(x+4);
}

static size_t offset_dotprod(const unsigned char *p, const unsigned char *offs, const unsigned char *b, size_t n)
{
    size_t rv = 0;
    for (size_t i = 0; i < n; i++)
        rv += read_i32(p + offs[i]) * b[i];
    return rv;
}

static void do_tzset(void)
{
    static char varbuf[32];
    static char *oldtz = varbuf;
    static size_t oldtzsize = sizeof varbuf;

    const char *tz = getenv("TZ");
    if (!tz) tz = "/etc/localtime";
    if (!*tz) tz = utc;
    if (oldtz && !strcmp(tz, oldtz)) return;

    size_t tzlen = strlen(tz);
    if (!oldtz || tzlen >= oldtzsize) {
        /* yes, I know this leaks, but I'll avoid free() to avoid pulling it
         * into static linking where it might be unwanted.
         * Most applications never change the TZ ever. Most TZs fit inside of varbuf.
         * Even if not, this is only reallocated once the next higher power of two is reached.
         */
        size_t l = oldtzsize;
        while (l <= tzlen) l *= 2;
        oldtz = __libc_malloc(l);
        oldtzsize = l;
    }
    if (oldtz) memcpy(oldtz, tz, tzlen + 1);

    if (infofile) {
        __munmap((void *)infofile, infosize);
        infofile = 0;
        infosize = 0;
    }
    if (!is_posix_form(tz)) {
        if (*tz == ':') tz++, tzlen--;
        if (is_tz_acceptable(tz)) {
            int fd = -1;
            if (tz[0] == '/')
                fd = __sys_open(tz, O_RDONLY | O_CLOEXEC);
            else {
                static const char *const searchpath[] = { "/usr/share/zoneinfo/", "/share/zoneinfo/", "/etc/zoneinfo/" };
                for (size_t i = 0; fd == -1 && i < sizeof searchpath / sizeof *searchpath; i++)
                {
                    size_t spl = strlen(searchpath[i]);
                    char pathbuf[spl + tzlen + 1];
                    memcpy(pathbuf, searchpath[i], spl);
                    memcpy(pathbuf + spl, tz, tzlen + 1);
                    fd = __sys_open(pathbuf, O_RDONLY | O_CLOEXEC);
                }
            }
            struct stat st;
            char magic[4];
            const unsigned char *p;
            if (fd != -1)
                __syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);

            if (fd != -1
                    && __syscall(SYS_read, fd, magic, 4) == 4
                    && !memcmp(magic, "TZif", 4)
                    && !__fstat(fd, &st)
                    && st.st_size < PTRDIFF_MAX
                    && (p = __mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) != MAP_FAILED) {
                infofile = p;
                infosize = st.st_size;
                p += 64 + offset_dotprod(p, (unsigned char[]){20,24,28,32,36,40}, (unsigned char[]){1,1,8,5,6,1}, 6);
                uint32_t isutcnt = read_i32(p);
                uint32_t isstdcnt = read_i32(p+4);
                uint32_t leapcnt = read_i32(p+8);
                uint32_t timecnt = read_i32(p+12);
                uint32_t typecnt = read_i32(p+16);
                uint32_t charcnt = read_i32(p+20);
                transitions = p + 24;
                transtypes = transitions + timecnt * 8;
                lttypes = transtypes + timecnt;
                abbrevs = (void *)(lttypes + typecnt * 6);
                tz = abbrevs + charcnt + leapcnt * 12 + isstdcnt + isutcnt + 1;
                maxtime = read_i64(transitions + (timecnt - 1) * 8);
            }
            if (fd != -1)
                __syscall(SYS_close, fd);
        }
    }
    if (tz && parse_posix_form(tz)) {
        __daylight = 0;
        __tzname[0] = (char *)utc;
        __timezone = 0;
    }
}

static void __tzset(void)
{
    __lock(&lock);
    do_tzset();
    __unlock(&lock);
}
weak_alias(tzset, __tzset);

static int num_days_in_month(int mon, int isleap)
{
    if (mon == 2) return 28 + isleap;
    return 30 + ((0xad5 >> (mon - 1)) & 1);
}
static time_t __rule_to_time(const struct rule *r, time_t yearstart, int isleap)
{
    if (r->type == GREGORIAN)
        return yearstart + r->u.day * 86400 + r->time;
    if (r->type == JULIAN)
        return yearstart + (r->u.day - 1 + (isleap && r->u.day > 59)) * 86400 + r->time;
    int daynum = yearstart / 86400;
    static const unsigned short monthstart[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int firstof = daynum + monthstart[r->u.para[0] - 1] + (isleap && r->u.para[0] > 2);
    int firstdowof = (r->u.para[2] - firstof - 4) % 7;
    if (firstdowof < 0) firstdowof += 7;
    int wantedday = firstdowof + (r->u.para[1] - 1) * 7;
    if (wantedday >= num_days_in_month(r->u.para[0], isleap))
        wantedday -= 7;
    return (firstof + wantedday) * 86400 + r->time;
}

static size_t find_transtype(long long time, int islocal)
{
    uint64_t t;
    size_t n = (transtypes - transitions)/8;
    size_t start = 0;
    size_t end = n;
    while (end > start + 1) {
        size_t mid = (end + start) / 2;
        t = read_i64(transitions + mid * 8);
        if (islocal) t += (int32_t)read_i32(lttypes + 6 * transtypes[mid]);
        if (time < t) end = mid;
        else start = mid;
    }
    if (start == 0 && time < read_i64(transitions) + (islocal? (int32_t)read_i32(lttypes + 6 * transtypes[0]) : 0))
        return 0;
    return transtypes[start];
}

static void get_std_tz_unlocked(struct tz *tz)
{
    tz->gmtoff = -__timezone;
    tz->isdst = 0;
    tz->name = __tzname[0];
}

static void get_dst_tz_unlocked(struct tz *tz)
{
    tz->gmtoff = -dstzone;
    tz->isdst = 1;
    tz->name = __tzname[1];
}

static void time_to_tz_unlocked(long long time, int islocal, struct tz *tz)
{
    do_tzset();
    if (infofile && time < maxtime)
    {
        size_t i = find_transtype(time, islocal);
        tz->gmtoff = (int32_t)read_i32(lttypes + 6 * i);
        tz->isdst = lttypes[6 * i + 4];
        tz->name = abbrevs + lttypes[6 * i + 5];
    } else if (__daylight) {
        long long year = __time_to_year(islocal? time + __timezone : time);
        int isleap;
        time_t yearstart = __year_to_time(year, &isleap);
        time_t t0 = __rule_to_time(&rules[0], yearstart, isleap);
        time_t t1 = __rule_to_time(&rules[1], yearstart, isleap);
        if (!islocal) {
            t0 += __timezone;
            t1 += dstzone;
        }
        if (t0 < t1) {
            if (time < t0 || time >= t1) get_std_tz_unlocked(tz);
            else get_dst_tz_unlocked(tz);
        } else {
            if (time < t1 || time >= t0) get_dst_tz_unlocked(tz);
            else get_std_tz_unlocked(tz);
        }
    } else {
        get_std_tz_unlocked(tz);
    }
}

hidden struct tz __time_to_tz(long long time, int islocal)
{
    struct tz tz;
    __lock(&lock);
    time_to_tz_unlocked(time, islocal, &tz);
    __unlock(&lock);
    return tz;
}

hidden struct tz __get_std_tz(void)
{
    struct tz tz;
    __lock(&lock);
    do_tzset();
    get_std_tz_unlocked(&tz);
    __unlock(&lock);
    return tz;
}

hidden struct tz __get_dst_tz(void)
{
    struct tz tz;
    __lock(&lock);
    do_tzset();
    if (__daylight)
        get_dst_tz_unlocked(&tz);
    else
        get_std_tz_unlocked(&tz);
    __unlock(&lock);
    return tz;
}
