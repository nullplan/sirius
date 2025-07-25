#ifndef STDIO_H
#define STDIO_H

#include "../../include/stdio.h"
#include <stdint.h>
#include <stdbool.h>

struct __localedef;
struct __file {
    int fd;                                             /* file descriptor */
    int flags;                                          /* F_ flags */
    unsigned char *buf;                                 /* pointer to buffer */
    unsigned char *pos;                                 /* current position in buffer */
    unsigned char *end;                                 /* end of buffer */
    size_t buf_size;                                    /* buffer size */
    int dir;                                            /* direction (0 - undirected, 1 - read, -1 - write) */
    int lbf;                                            /* line buffering flag ('\n' - line buffered, -1 - not line buffered) */
    ssize_t (*read)(FILE *, void *, size_t);            /* reads to arg buffer first, then file buffer */
    size_t (*write)(FILE *, const void *, size_t);      /* writes from file buffer first, then arg buffer */
    off_t (*seek)(FILE *, off_t, int);                  /* seeks and returns new offset */
    int (*close)(FILE *);                               /* closes and does whatever */
    void *cookie;                                       /* miscellaneous stuff for other file types */
    FILE *next, *prev;                                  /* list pointers for file list. */
    FILE *lnext, *lprev;                                /* list pointers for lock list. */
    volatile int lock;                                  /* -1 - file doesn't need locks, 0 - free, other - owned by given thread. */
    unsigned count;                                     /* recursive lock count */
    int waiters;                                        /* waiters count */
    int popen_pid;                                      /* pid for pclose() */
    off_t shlim;                                        /* limit for the scan-helper functions. */
    off_t shcnt;                                        /* number of read characters since last shlim */
    unsigned char *shend;                               /* pointer to end for scan helpers */
    int orientation;                                    /* orientation for fwide() */
    const struct __localedef *locale;                   /* locale for MB functions */
};

#define UNGET       8       /* number of bytes for ungetc(), reserved in front of buffer */

#define F_NORD      1       /* no reading allowed */
#define F_NOWR      2       /* no writing allowed */
#define F_ERR       4       /* error flag */
#define F_EOF       8       /* end-of-file flag */
#define F_PERM      16      /* file was not allocated (so don't free it) */
#define F_SETBUF    32      /* setvbuf() executed (don't automatically change buffer mode) */

extern hidden void __stdio_list_init(void);     /* initializes open-file-list with used standard files. */
extern hidden void __stdio_init_locks(void);    /* initializes locks in currently open files. */
extern hidden void __stdio_exit(void);          /* flushes all open files, leaves stdio unusable */
extern hidden void __stdio_exit_needed(void);   /* alias to force correct link */
extern hidden FILE *__ofl_lock(void);           /* locks open-file-list, returns head pointer */
extern hidden void __ofl_unlock(FILE *);        /* sets head pointer to argument, unlocks OFL */

extern hidden int __toread(FILE *);             /* switches file to read mode if needed, returns 0 if successful, EOF if not possible */
extern hidden int __towrite(FILE *);            /* same for write mode */

extern hidden int __overflow(FILE *, int);      /* writes second arg to file; called in case of buffer overflow. */
extern hidden int __underflow(FILE *);          /* returns next byte from FILE or else EOF, refills buffer */

#define IS_READ(f)  ((f)->dir > 0)
#define IS_WRITE(f) ((f)->dir < 0)

#define getc_unlocked(f)    (IS_READ(f) && (f)->pos < (f)->end? *(f)->pos++ : __underflow(f))
#define putc_unlocked(c, f) (IS_WRITE(f) && (f)->pos < (f)->end && (c) != (f)->lbf? *(f)->pos++ = (c) : __overflow((f), (c)))

extern hidden ssize_t __stdio_read(FILE *, void *, size_t);
extern hidden size_t __stdio_write(FILE *, const void *, size_t);
extern hidden off_t __stdio_seek(FILE *, off_t, int);
extern hidden int __stdio_close(FILE *);

extern hidden int __flockfile(FILE *);
extern hidden void __funlockfile(FILE *);

#define __FLOCK(f) int __needs_unlock = (f)->lock > -1 && __flockfile(f)
#define __FUNLOCK(f) ((__needs_unlock)? __funlockfile(f) : (void)0)

extern hidden size_t __fwritex(const void *restrict, size_t, FILE *restrict);
extern hidden FILE *__fdopen_flg(int, int);
extern hidden int __fopen_flags(const char *);

extern hidden int __shlim(FILE *, off_t);
extern hidden int __shgetc(FILE *);

#define sh_fromstr(f, s) ((f)->dir = 1, (f)->buf = (f)->pos = (void *)(s), (f)->end = (void *)(-1))
#define shlim(f, c) __shlim((f), (c))
#define shgetc(f) (IS_READ(f) && (f)->pos != (f)->shend? *(f)->pos++ : __shgetc(f))
#define shungetc(f) (IS_READ(f) && (f)->pos > (f)->buf? (f)->pos-- : 0)
#define shcnt(f) ((f)->shcnt + ((f)->pos - (f)->buf))

extern hidden uintmax_t __intscan(FILE *, int, uintmax_t);
extern hidden long double __floatscan(FILE *, int, int, int);

extern hidden void __register_locked_file(FILE *);

enum format {
    fmt_float,
    fmt_double,
    fmt_ldouble
};

/* Serendipity: All flags characters are within 32 code points of the space */
#define FLG(x)  (1ul << ((x) - ' '))
#define FLG_THOU    FLG('\'')
#define FLG_LEFT    FLG('-')
#define FLG_SIGN    FLG('+')
#define FLG_MARK    FLG(' ')
#define FLG_ZERO    FLG('0')
#define FLG_ALT     FLG('#')
#define ALL_FLAGS (FLG_THOU | FLG_LEFT | FLG_SIGN | FLG_MARK | FLG_ZERO | FLG_ALT)

extern hidden size_t __fmt_fp(FILE *f, long double x, int width, int prec, int flags, int c, enum format);
#endif
