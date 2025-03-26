#ifndef __UNISTD_H
#define __UNISTD_H

#define __NEED_NULL
#define __NEED_SEEK_SET
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_pid_t
#define __NEED_off_t
#define __NEED_intptr_t
#include <alltypes.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif


#define _POSIX_VERSION      202405L
#define _POSIX2_VERSION     _POSIX_VERSION
#define _XOPEN_VERSION      800L
#define _POSIX_ADVISORY_INFO    _POSIX_VERSION
#define _POSIX_ASYNCHRONOUS_IO  _POSIX_VERSION
#define _POSIX_BARRIERS         _POSIX_VERSION
#define _POSIX_CHOWN_RESTRICTED _POSIX_VERSION
#define _POSIX_CLOCK_SELECTION  _POSIX_VERSION
#define _POSIX_CPUTIME          _POSIX_VERSION
#define _POSIX_DEVICE_CONTROL   _POSIX_VERSION
#define _POSIX_FSYNC            _POSIX_VERSION
#define _POSIX_IPV6             _POSIX_VERSION
#define _POSIX_JOB_CONTROL      _POSIX_VERSION
#define _POSIX_MAPPED_FILES     _POSIX_VERSION
#define _POSIX_MEMLOCK          _POSIX_VERSION
#define _POSIX_MEMLOCK_RANGE    _POSIX_VERSION
#define _POSIX_MEMORY_PROTECTION _POSIX_VERSION
#define _POSIX_MESSAGE_PASSING  _POSIX_VERSION
#define _POSIX_MONOTONIC_CLOCK  _POSIX_VERSION
#define _POSIX_NO_TRUNC         _POSIX_VERSION
#define _POSIX_RAW_SOCKETS      _POSIX_VERSION
#define _POSIX_READER_WRITER_LOCKS _POSIX_VERSION
#define _POSIX_REALTIME_SIGNALS _POSIX_VERSION
#define _POSIX_REGEXP           _POSIX_VERSION
#define _POSIX_SAVED_IDS        _POSIX_VERSION
#define _POSIX_SEMAPHORES       _POSIX_VERSION
#define _POSIX_SHARED_MEMORY_OBJECTS _POSIX_VERSION
#define _POSIX_SHELL            _POSIX_VERSION
#define _POSIX_SPAWN            _POSIX_VERSION
#define _POSIX_SPIN_LOCKS       _POSIX_VERSION
#define _POSIX_SYNCHRONIZED_IO  _POSIX_VERSION
#define _POSIX_THREAD_ATTR_STACKADDR _POSIX_VERSION
#define _POSIX_THREAD_ATTR_STACKSIZE _POSIX_VERSION
#define _POSIX_THREAD_CPUTIME   _POSIX_VERSION
#define _POSIX_THREAD_PRIO_INHERIT _POSIX_VERSION
#define _POSIX_THREAD_PRIORITY_SCHEDULING _POSIX_VERSION
#define _POSIX_THREAD_PROCESS_SHARED _POSIX_VERSION
#define _POSIX_THREAD_ROBUST_PRIO_INHERIT _POSIX_VERSION
#define _POSIX_THREAD_SAFE_FUNCTIONS _POSIX_VERSION
#define _POSIX_THREADS          _POSIX_VERSION
#define _POSIX_TIMEOUTS         _POSIX_VERSION
#define _POSIX_TIMERS           _POSIX_VERSION
#define _POSIX2_C_BIND          _POSIX_VERSION
#define _POSIX2_C_DEV           _POSIX_VERSION

#ifdef __SIXTY_FOUR
#define _POSIX_V7_LP64_OFF64    _POSIX_VERSION
#define _POSIX_V7_LP64_OFFBIG   _POSIX_VERSION
#define _POSIX_V8_LP64_OFF64    _POSIX_VERSION
#define _POSIX_V8_LP64_OFFBIG   _POSIX_VERSION
#else
#define _POSIX_V7_ILP32_OFFBIG  _POSIX_VERSION
#define _POSIX_V8_ILP32_OFFBIG  _POSIX_VERSION
#endif

#define _XOPEN_CRYPT    _XOPEN_VERSION
#define _XOPEN_ENH_I18N _XOPEN_VERSION
#define _XOPEN_SHM      _XOPEN_VERSION
#define _XOPEN_UNIX     _XOPEN_VERSION

#define F_OK        0
#define R_OK        4
#define W_OK        2
#define X_OK        1

enum {
    _CS_PATH,

    _CS_POSIX_V8_ILP32_OFF32_CFLAGS,
    _CS_POSIX_V8_ILP32_OFF32_LDFLAGS,
    _CS_POSIX_V8_ILP32_OFF32_LIBS,
    _CS_POSIX_V8_ILP32_OFFBIG_CFLAGS,
    _CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS,
    _CS_POSIX_V8_ILP32_OFFBIG_LIBS,
    _CS_POSIX_V8_LP64_OFF64_CFLAGS,
    _CS_POSIX_V8_LP64_OFF64_LDFLAGS,
    _CS_POSIX_V8_LP64_OFF64_LIBS,
    _CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS,
    _CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS,
    _CS_POSIX_V8_LPBIG_OFFBIG_LIBS,
    _CS_POSIX_V8_THREADS_CFLAGS,
    _CS_POSIX_V8_THREADS_LDFLAGS,
    _CS_POSIX_V8_WIDTH_RESTRICTED_ENVS,
    _CS_V8_ENV,
    _CS_POSIX_V7_ILP32_OFF32_CFLAGS,
    _CS_POSIX_V7_ILP32_OFF32_LDFLAGS,
    _CS_POSIX_V7_ILP32_OFF32_LIBS,
    _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS,
    _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS,
    _CS_POSIX_V7_ILP32_OFFBIG_LIBS,
    _CS_POSIX_V7_LP64_OFF64_CFLAGS,
    _CS_POSIX_V7_LP64_OFF64_LDFLAGS,
    _CS_POSIX_V7_LP64_OFF64_LIBS,
    _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS,
    _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS,
    _CS_POSIX_V7_LPBIG_OFFBIG_LIBS,
    _CS_POSIX_V7_THREADS_CFLAGS,
    _CS_POSIX_V7_THREADS_LDFLAGS,
    _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS,
    _CS_V7_ENV,
};

#define SEEK_DATA       3
#define SEEK_HOLE       4

enum {
    F_LOCK,
    F_TEST,
    F_TLOCK,
    F_ULOCK,
};

enum {
    _PC_2_SYMLINKS,
    _PC_ALLOC_SIZE_MIN,
    _PC_ASYNC_IO,
    _PC_CHOWN_RESTRICTED,
    _PC_FALLOC,
    _PC_FILESIZEBITS,
    _PC_LINK_MAX,
    _PC_MAX_CANON,
    _PC_MAX_INPUT,
    _PC_NAME_MAX,
    _PC_NO_TRUNC,
    _PC_PATH_MAX,
    _PC_PIPE_BUF,
    _PC_PRIO_IO,
    _PC_REC_INCR_XFER_SIZE,
    _PC_REC_MAX_XFER_SIZE,
    _PC_REC_MIN_XFER_SIZE,
    _PC_REC_XFER_ALIGN,
    _PC_SYMLINK_MAX,
    _PC_SYNC_IO,
    _PC_TEXTDOMAIN_MAX,
    _PC_TIMESTAMP_RESOLUTION,
    _PC_VDISABLE,
};

enum {
    _SC_2_C_BIND,
    _SC_2_C_DEV,
    _SC_2_CHAR_TERM,
    _SC_2_FORT_RUN,
    _SC_2_LOCALEDEF,
    _SC_2_SW_DEV,
    _SC_2_UPE,
    _SC_2_VERSION,
    _SC_ADVISORY_INFO,
    _SC_AIO_LISTIO_MAX,
    _SC_AIO_MAX,
    _SC_AIO_PRIO_DELTA_MAX,
    _SC_ARG_MAX,
    _SC_ASYNCHRONOUS_IO,
    _SC_ATEXIT_MAX,
    _SC_BARRIERS,
    _SC_BC_BASE_MAX,
    _SC_BC_DIM_MAX,
    _SC_BC_SCALE_MAX,
    _SC_BC_STRING_MAX,
    _SC_CHILD_MAX,
    _SC_CLK_TCK,
    _SC_CLOCK_SELECTION,
    _SC_COLL_WEIGHTS_MAX,
    _SC_CPUTIME,
    _SC_DELAYTIMER_MAX,
    _SC_DEVICE_CONTROL,
    _SC_EXPR_NEST_MAX,
    _SC_FSYNC,
    _SC_GETGR_R_SIZE_MAX,
    _SC_GETPW_R_SIZE_MAX,
    _SC_HOST_NAME_MAX,
    _SC_IOV_MAX,
    _SC_IPV6,
    _SC_JOB_CONTROL,
    _SC_LINE_MAX,
    _SC_LOGIN_NAME_MAX,
    _SC_MAPPED_FILES,
    _SC_MEMLOCK,
    _SC_MEMLOCK_RANGE,
    _SC_MEMORY_PROTECTION,
    _SC_MESSAGE_PASSING,
    _SC_MONOTONIC_CLOCK,
    _SC_MQ_OPEN_MAX,
    _SC_MQ_PRIO_MAX,
    _SC_NGROUPS_MAX,
    _SC_NPROCESSORS_CONF,
    _SC_NPROCESSORS_ONLN,
    _SC_NSIG,
    _SC_OPEN_MAX,
    _SC_PAGE_SIZE,
    _SC_PAGESIZE,
    _SC_PRIORITIZED_IO,
    _SC_PRIORITY_SCHEDULING,
    _SC_RAW_SOCKETS,
    _SC_RE_DUP_MAX,
    _SC_READER_WRITER_LOCKS,
    _SC_REALTIME_SIGNALS,
    _SC_REGEXP,
    _SC_RTSIG_MAX,
    _SC_SAVED_IDS,
    _SC_SEM_NSEMS_MAX,
    _SC_SEM_VALUE_MAX,
    _SC_SEMAPHORES,
    _SC_SHARED_MEMORY_OBJECTS,
    _SC_SHELL,
    _SC_SIGQUEUE_MAX,
    _SC_SPAWN,
    _SC_SPIN_LOCKS,
    _SC_SPORADIC_SERVER,
    _SC_SS_REPL_MAX,
    _SC_STREAM_MAX,
    _SC_SYMLOOP_MAX,
    _SC_SYNCHRONIZED_IO,
    _SC_THREAD_ATTR_STACKADDR,
    _SC_THREAD_ATTR_STACKSIZE,
    _SC_THREAD_CPUTIME,
    _SC_THREAD_DESTRUCTOR_ITERATIONS,
    _SC_THREAD_KEYS_MAX,
    _SC_THREAD_PRIO_INHERIT,
    _SC_THREAD_PRIO_PROTECT,
    _SC_THREAD_PRIORITY_SCHEDULING,
    _SC_THREAD_PROCESS_SHARED,
    _SC_THREAD_ROBUST_PRIO_INHERIT,
    _SC_THREAD_ROBUST_PRIO_PROTECT,
    _SC_THREAD_SAFE_FUNCTIONS,
    _SC_THREAD_SPORADIC_SERVER,
    _SC_THREAD_STACK_MIN,
    _SC_THREAD_THREADS_MAX,
    _SC_THREADS,
    _SC_TIMEOUTS,
    _SC_TIMER_MAX,
    _SC_TIMERS,
    _SC_TTY_NAME_MAX,
    _SC_TYPED_MEMORY_OBJECTS,
    _SC_TZNAME_MAX,
    _SC_V8_ILP32_OFF32,
    _SC_V8_ILP32_OFFBIG,
    _SC_V8_LP64_OFF64,
    _SC_V8_LPBIG_OFFBIG,
    _SC_V7_ILP32_OFF32,
    _SC_V7_ILP32_OFFBIG,
    _SC_V7_LP64_OFF64,
    _SC_V7_LPBIG_OFFBIG,
    _SC_VERSION,
    _SC_XOPEN_CRYPT,
    _SC_XOPEN_ENH_I18N,
    _SC_XOPEN_REALTIME,
    _SC_XOPEN_REALTIME_THREADS,
    _SC_XOPEN_SHM,
    _SC_XOPEN_UNIX,
    _SC_XOPEN_UUCP,
    _SC_XOPEN_VERSION,
};

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#define _POSIX_VDISABLE 0
#define POSIX_CLOSE_RESTART 0

extern char *optarg;
extern int opterr, optind, optopt;

int access(const char *, int);
unsigned alarm(unsigned);
int chdir(const char *);
int chown(const char *, uid_t, gid_t);
int close(int);
size_t confstr(int, char *, size_t);
int dup(int);
int dup2(int, int);
int dup3(int, int, int);
_Noreturn void _exit(int);
int execl(const char *, const char *, ...);
int execle(const char *, const char *, ...);
int execlp(const char *, const char *, ...);
int execv(const char *, char *const []);
int execve(const char *, char *const [], char *const []);
int execvp(const char *, char *const []);
int faccessat(int, const char *, int, int);
int fchdir(int);
int fchown(int, uid_t, gid_t);
int fchownat(int, const char *, uid_t, gid_t, int);
int fdatasync(int);
int fexecve(int, char *const [], char *const []);
pid_t _Fork(void);
pid_t fork(void);
long fpathconf(int, int);
int fsync(int);
int ftruncate(int, off_t);
char *getcwd(char *, size_t);
gid_t getegid(void);
int getentropy(void *, size_t);
uid_t geteuid(void);
gid_t getgid(void);
uid_t getuid(void);
int getgroups(int, gid_t []);
int gethostname(char *, size_t);
char *getlogin(void);
int getlogin_r(char *, size_t);
int getopt(int, char *const [], const char *);
pid_t getpgid(pid_t);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
pid_t getsid(pid_t);
int isatty(int);
int lchown(const char *, uid_t, gid_t);
int link(const char *, const char *);
int linkat(int, const char *, int, const char *, int);
off_t lseek(int, off_t, int);
long pathconf(const char *, int);
int pause(void);
int pipe(int [2]);
int pipe2(int [2], int);
int posix_close(int, int);
ssize_t pread(int, void *, size_t, off_t);
ssize_t pwrite(int, const void *, size_t, off_t);
ssize_t read(int, void *, size_t);
ssize_t readlink(const char *__restrict, char *__restrict, size_t);
ssize_t readlinkat(int, const char *__restrict, char *__restrict, size_t);
int rmdir(const char *);
int setegid(gid_t);
int seteuid(uid_t);
int setgid(gid_t);
int setuid(uid_t);
int setpgid(pid_t, pid_t);
pid_t setsid(void);
unsigned sleep(unsigned);
int symlink(const char *, const char *);
int symlinkat(const char *, int, const char *);
long sysconf(int);
pid_t tcgetpgrp(int);
int tcsetpgrp(int, pid_t);
int truncate(const char *, off_t);
char *ttyname(int);
int ttyname_r(int, char *, size_t);
int unlink(const char *);
int unlinkat(int, const char *, int);
ssize_t write(int, const void *, size_t);

#ifdef _XOPEN_SOURCE
char *crypt(const char *, const char *);
int gethostid(void);
int getresgid(gid_t *__restrict, gid_t *__restrict, gid_t *__restrict);
int getresuid(uid_t *__restrict, uid_t *__restrict, uid_t *__restrict);
int lockf(int, int, off_t);
int nice(int);
int setregid(gid_t, gid_t);
int setreuid(uid_t, uid_t);
int setresgid(gid_t, gid_t, gid_t);
int setresuid(uid_t, uid_t, uid_t);
void swab(const void *__restrict, void *__restrict, size_t);
void sync(void);
#endif
#ifdef __cplusplus
}
#endif
#endif
