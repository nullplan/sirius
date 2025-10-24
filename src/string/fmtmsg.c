#include <fmtmsg.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int full_writev(int fd, struct iovec *iov, size_t n)
{
    while (n) {
        ssize_t written = writev(fd, iov, n);
        if (written < 0) return -1;
        while (n && written >= iov->iov_len) {
            written -= iov->iov_len;
            n--;
            iov++;
        }
        if (n) {
            iov->iov_base = (char *)iov->iov_base + written;
            iov->iov_len -= written;
        }
    }
    return 0;
}

#define HAS_LABEL       1
#define HAS_SEVERITY    2
#define HAS_TEXT        4
#define HAS_ACTION      8
#define HAS_TAG         16
#define HAS_ALL         31

static const struct keyword {
    char str[11];
    unsigned char len;
    int val;
} keywords[] = {
    {"label", 5, HAS_LABEL},
    {"severity", 8, HAS_SEVERITY},
    {"text", 4, HAS_TEXT},
    {"action", 6, HAS_ACTION},
    {"tag", 3, HAS_TAG},
};
#define NKEY (sizeof keywords / sizeof *keywords)

static int parse_verbosity(void)
{
    const char *str = getenv("MSGVERB");
    if (!str || !*str) return HAS_ALL;
    int rv = 0;
    while (*str)
    {
        size_t len = __stridx(str, ':');
        const struct keyword *k;
        for (k = keywords; k < keywords + NKEY; k++)
            if (len == k->len && !memcmp(str, k->str, k->len))
                break;
        if (k == keywords + NKEY) return HAS_ALL;
        rv |= k->val;
        str += len;
        if (*str) str++;
    }
    return rv;
}

static int construct_message(struct iovec *iov, int verb, const char *label, int sev, const char *text, const char *action, const char *tag)
{
    static const char *const sevstr[] = {
        [MM_HALT] = "HALT",
        [MM_ERROR] = "ERROR",
        [MM_WARNING] = "WARNING",
        [MM_INFO] = "INFO",
    };
    int n;
    n = 0;
    if ((verb & HAS_LABEL) && label) {
        iov[n].iov_base = (void *)label;
        iov[n].iov_len = strlen(label);
        iov[n+1].iov_base = ": ";
        iov[n+1].iov_len = 2;
        n += 2;
    }
    if ((verb & HAS_SEVERITY) && sevstr[sev]) {
        iov[n].iov_base = (void *)sevstr[sev];
        iov[n].iov_len = strlen(sevstr[sev]);
        iov[n+1].iov_base = ": ";
        iov[n+1].iov_len = 2;
        n += 2;
    }
    if ((verb & HAS_TEXT) && text) {
        iov[n].iov_base = (void *)text;
        iov[n].iov_len = strlen(text);
        n++;
    }
    if ((verb & HAS_ACTION) && action) {
        iov[n].iov_base = "\nTO FIX: ";
        iov[n].iov_len = 9;
        iov[n+1].iov_base = (void *)action;
        iov[n+1].iov_len = strlen(action);
        n += 2;
    }
    if ((verb & HAS_TAG) && tag) {
        iov[n].iov_base = " ";
        iov[n].iov_len = 1;
        iov[n+1].iov_base = (void *)tag;
        iov[n+1].iov_len = strlen(tag);
        n += 2;
    }
    if (n) {
        iov[n].iov_base = "\n";
        iov[n].iov_len = 1;
        n++;
    }
    return n;
}

int fmtmsg(long class, const char *label, int sev, const char *text, const char *action, const char *tag)
{
    struct iovec iov[10];
    int n;
    int ret = 0;
    if (sev > 4u) return MM_NOTOK;
    if (class & MM_CONSOLE)
    {
        int fd = open("/dev/console", O_WRONLY | O_CLOEXEC);
        if (fd >= 0) {
            n = construct_message(iov, HAS_ALL, label, sev, text, action, tag);
            if (full_writev(fd, iov, n))
                ret |= MM_NOCON;
            if (close(fd))
                ret |= MM_NOCON;
        }
    }
    if (class & MM_PRINT)
    {
        /* construct message again */
        int verbosity = parse_verbosity();
        n = construct_message(iov, verbosity, label, sev, text, action, tag);
        if (full_writev(2, iov, n))
            ret |= MM_NOMSG;
    }
    return ret;
}
