#include <ftw.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
struct history {
    dev_t dev;
    ino_t ino;
    const struct history *next;
};

static int is_in_history(dev_t dev, ino_t ino, const struct history *h)
{
    for (; h; h = h->next)
        if (dev == h->dev && ino == h->ino)
            return 1;
    return 0;
}

static int recurse(char *buf, int len, int (*cb)(const char *, const struct stat *, int, struct FTW *), int lim, int flags, int level, const struct history *hist)
{
    struct stat st;
    int type;
    int rv = (flags & FTW_PHYS? lstat : stat)(buf, &st);
    if (rv) {
        if (errno != EACCES)
            return -1;
        type = FTW_NS;
    } else if (S_ISLNK(st.st_mode)) {
        type = FTW_SL;
    } else if (S_ISDIR(st.st_mode)) {
        type = FTW_D;
    } else {
        type = FTW_F;
    }

    if (type == FTW_SL) {
        struct stat discard;
        if (!(flags & FTW_PHYS) || stat(buf, &discard)) {
            type = FTW_SLN;
        }
    }

    if (type != FTW_D) return cb(buf, &st, type, &(struct FTW){.base = len, .level = level});
    if (hist && (flags & FTW_MOUNT) && hist->dev != st.st_dev) return 0;

    int fd = open(buf, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd == -1) {
        if (errno != EACCES)
            return -1;
        return cb(buf, &st, FTW_DNR, &(struct FTW){.base = len, .level = level});
    } else if (!(flags & FTW_DEPTH)) {
        rv = cb(buf, &st, FTW_D, &(struct FTW){.base = len, .level = level});
        if (rv) {
            close(fd);
            return rv;
        }
    }

    if (hist && (flags & FTW_XDEV) && st.st_dev != hist->dev) {
        if (flags & FTW_DEPTH) rv = cb(buf, &st, FTW_DP, &(struct FTW){.base = len, .level = level});
        close(fd);
        return 0;
    }
    if (level >= lim) {
        close(fd);
        return 0;
    }
    if (hist && is_in_history(st.st_dev, st.st_ino, hist)) {
        close(fd);
        return 0;
    }
    struct history h = {.dev = st.st_dev, .ino = st.st_ino, .next = hist};
    DIR *d = fdopendir(fd);
    if (!d) {
        close(fd);
        return -1;
    }

    size_t fulllen = len + strlen(buf + len);
    struct dirent *de;
    buf[fulllen] = '/';
    while ((de = readdir(d))) {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
        size_t namelen = strlen(de->d_name);
        if (namelen > NAME_MAX || namelen >= PATH_MAX - fulllen - 1) {
            errno = ENAMETOOLONG;
            rv = -1;
            break;
        }
        memcpy(buf + fulllen + 1, de->d_name, namelen + 1);
        rv = recurse(buf, fulllen + 1, cb, lim, flags, level + 1, &h);
        if (rv) break;
    }
    closedir(d);
    if (!rv && (flags & FTW_DEPTH)) {
        buf[fulllen] = 0;
        rv = cb(buf, &st, FTW_DP, &(struct FTW){.base = len, .level = level});
    }
    return rv;
}
int nftw(const char *dir, int (*cb)(const char *, const struct stat *, int, struct FTW *), int lim, int flags)
{
    char buf[PATH_MAX];

    size_t len = strnlen(dir, sizeof buf);
    if (len == 0) {
        errno = ENOENT;
        return -1;
    }
    if (len >= sizeof buf) {
        errno = ENAMETOOLONG;
        return -1;
    }
    const char *p = strrchr(dir, '/');
    int base = p? p - dir + 1 : 0;
    return recurse(memcpy(buf, dir, len + 1), base, cb, lim, flags, 0, 0);
}
