#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/uio.h>
#include "libc.h"

static struct lock lock;
static int logfd = -1;
static int logmask = LOG_UPTO(LOG_DEBUG);
static int logfac = LOG_USER;
static int logopt;
static const char nilvalue[] = "-";
static const char *logappname = nilvalue;

void closelog(void)
{
    __lock(&lock);
    int cs;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    if (logfd != -1)
        close(logfd);
    logfd = -1;
    pthread_setcancelstate(cs, 0);
    __unlock(&lock);
}

static const struct {
    sa_family_t fam;
    char path[9];
} logaddress = {AF_UNIX, "/dev/log"};

static void openlog_internal(void)
{
    /* assumes lock is taken, cancellation is disabled, and logfd == -1 */
    int fd = socket(AF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (fd != -1 && connect(fd, (struct sockaddr *)&logaddress, sizeof logaddress) == 0)
        logfd = fd;
    if (fd != -1)
        close(fd);
}

void openlog(const char *ident, int opt, int facility)
{
    __lock(&lock);
    int cs;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    if (!(facility & ~LOG_FACMASK))
        logfac = facility;
    logopt = opt;
    if (ident)
        logappname = ident;
    else
        logappname = nilvalue;
    if (logfd != -1)
        close(logfd);
    if (!(opt & LOG_ODELAY))
        openlog_internal();
    pthread_setcancelstate(cs, 0);
    __unlock(&lock);
}

int setlogmask(int mask)
{
    __lock(&lock);
    int rv = logmask;
    if (mask)
        logmask = mask;
    __unlock(&lock);
    return rv;
}

static void vsyslog_internal(int pri, const char *msg, va_list ap, int entry_errno)
{
    if (!(LOG_MASK(LOG_PRI(pri)) & logmask))
        return;

    char statbuf[1024];
    char *buf = statbuf;
    errno = entry_errno;
    int msglen = vsnprintf(statbuf, sizeof statbuf, msg, ap);
    if (msglen >= sizeof statbuf)
    {
        buf = malloc(msglen + 1);
        if (!buf) return;
        errno = entry_errno;
        vsnprintf(buf, msglen + 1, msg, ap);
    }
    if (logfd == -1)
        openlog_internal();
    if (logfd == -1 && !(logopt & LOG_CONS))
        goto out;

    struct timespec ts;
    struct tm lt, *rv = 0;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        rv = localtime_r(&ts.tv_sec, &lt);

    char tsbuf[sizeof "2025-09-04T14:40:52.012345678+02:00 "];
    int tslen = 1;
    tsbuf[0] = '-';
    tsbuf[1] = ' ';
    tsbuf[2] = 0;
    if (rv)
    {
        int offhours = -lt.__tm_gmtoff / 3600;
        int offminutes = (-lt.__tm_gmtoff / 60) % 60;
        if (offminutes < 0) {
            offminutes += 60;
            offhours--;
        }
        tslen = snprintf(tsbuf, sizeof tsbuf, "%d-%02d-%02dT%02d:%02d:%02d.%09ld%+02d:%02d ",
                lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec, offhours, offminutes);
    }

    char headbuf[sizeof "<192>1 "];
    if (!(pri & LOG_FACMASK))
        pri |= logfac;
    int headlen = snprintf(headbuf, sizeof headbuf, "<%d>1 ", pri);

    const char *pn = __progname_short;
    char pidbuf[3 * sizeof (pid_t) + 3];
    int pidlen = 0;
    if (logopt & LOG_PID)
        pidlen = snprintf(pidbuf, sizeof pidbuf, "[%d]", getpid());
    if (!*pn && !pidlen)
        pn = nilvalue;

    char hnbuf[256];
    const char *hn = hnbuf;
    if (gethostname(hnbuf, sizeof hnbuf) || !*hnbuf) hn = nilvalue;
    char sp = ' ';

    struct iovec iov[12];
    iov[0].iov_base = headbuf;
    iov[0].iov_len = headlen;
    iov[1].iov_base = tsbuf;
    iov[1].iov_len = tslen;
    iov[2].iov_base = hnbuf;
    iov[2].iov_len = strlen(hnbuf);
    iov[3].iov_base = &sp;
    iov[3].iov_len = 1;
    iov[4].iov_base = (void *)logappname;
    iov[4].iov_len = strlen(logappname);
    iov[5].iov_base = &sp;
    iov[5].iov_len = 1;
    iov[6].iov_base = (void *)pn;
    iov[6].iov_len = strlen(pn);
    iov[7].iov_base = pidbuf;
    iov[7].iov_len = pidlen;
    iov[8].iov_base = &sp;
    iov[8].iov_len = 1;
    iov[9].iov_base = "- - "; /* MSGID and STRUCTURED-DATA both empty */
    iov[9].iov_len = 4;
    iov[10].iov_base = buf;
    iov[10].iov_len = msglen;

    int sendsuccess = 0;
    if (logfd != -1) {
        struct msghdr msg = {0};
        msg.msg_iov = iov;
        msg.msg_iovlen = 11;
        ssize_t total_len = 0;
        for (size_t i = 0; i < 11; i++)
            total_len += iov[i].iov_len;
        sendsuccess = sendmsg(logfd, &msg, MSG_NOSIGNAL) == total_len;
    }
    if ((logopt & LOG_CONS) && !sendsuccess) {
        int consfd = open("/dev/console", O_WRONLY | O_CLOEXEC | O_NOCTTY);
        if (consfd != -1) {
            iov[11].iov_base = "\r\n";
            iov[11].iov_len = 2;
            writev(consfd, iov, 12);
            close(consfd);
        }
    }

out:
    if (buf != statbuf)
        free(buf);
}
void syslog(int pri, const char *msg, ...)
{
    int e = errno;
    __lock(&lock);
    int cs;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    va_list ap;
    va_start(ap, msg);
    vsyslog_internal(pri, msg, ap, e);
    va_end(ap);
    pthread_setcancelstate(cs, 0);
    __unlock(&lock);
}
