#!/bin/sh

[ "$CC" ] || CC=cc
if ! [ "$ARCH" ]; then
    printf "%s: ARCH variable required" "$0" >&2
    exit 1
fi

C99_HEADERS="assert.h complex.h ctype.h errno.h fenv.h float.h inttypes.h iso646.h limits.h locale.h math.h setjmp.h signal.h stdarg.h stdbool.h stddef.h stdint.h stdio.h stdlib.h string.h time.h wchar.h wctype.h"
C11_HEADERS="$C99_HEADERS tgmath.h stdalign.h stdnoreturn.h threads.h uchar.h"
C23_HEADERS="$C11_HEADERS"
POSIX_HEADERS="$C23_HEADERS
aio.h
arpa/inet.h
cpio.h
devctl.h
dirent.h
dlfcn.h
endian.h
fcntl.h
fnmatch.h
glob.h
grp.h
iconv.h
langinfo.h
libintl.h
monetary.h
mqueue.h
net/if.h
netdb.h
netinet/in.h
netinet/tcp.h
nl_types.h
poll.h
pthread.h
pwd.h
regex.h
sched.h
semaphore.h
spawn.h
strings.h
sys/mman.h
sys/resource.h
sys/select.h
sys/socket.h
sys/stat.h
sys/statvfs.h
sys/times.h
sys/types.h
sys/un.h
sys/utsname.h
sys/wait.h
tar.h
termios.h
unistd.h
wordexp.h"
XSI_HEADERS="$POSIX_HEADERS
fmtmsg.h
ftw.h
libgen.h
search.h
sys/ipc.h
sys/msg.h
sys/sem.h
sys/shm.h
sys/time.h
sys/uio.h
syslog.h
utmpx.h"

set -e
for i in $C99_HEADERS; do
    printf "#include <%s>\n" "$i" >tmp.c
    $CC -nostdinc -isystem include -isystem arch/$ARCH -isystem obj/include -std=c99 -c tmp.c -o /dev/null
done

for i in $C11_HEADERS; do
    printf "#include <%s>\n" "$i" >tmp.c
    $CC -nostdinc -isystem include -isystem arch/$ARCH -isystem obj/include -std=c11 -c tmp.c -o /dev/null
done

for i in $C23_HEADERS; do
    printf "#include <%s>\n" "$i" >tmp.c
    $CC -nostdinc -isystem include -isystem arch/$ARCH -isystem obj/include -std=c2x -c tmp.c -o /dev/null
done

for i in $POSIX_HEADERS; do
    printf "#define _POSIX_C_SOURCE\n#include <%s>\n" "$i" >tmp.c
    $CC -nostdinc -isystem include -isystem arch/$ARCH -isystem obj/include -std=c99 -c tmp.c -o /dev/null
done

for i in $XSI_HEADERS; do
    printf "#define _XOPEN_SOURCE\n#include <%s>\n" "$i" >tmp.c
    $CC -nostdinc -isystem include -isystem arch/$ARCH -isystem obj/include -std=c99 -c tmp.c -o /dev/null
done

