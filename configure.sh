#!/bin/sh

[ "$CC" ] || CC=cc
[ "$AR" ] || AR=ar

srcdir="${0%/*}"

if [ "$srcdir" != "${srcdir% *}" ]; then
    echo "No spaces in the source path!" >&2
    exit 1
fi

for i; do
    case "$i" in
        CC=*) CC="${i#CC=}" ;;
        AR=*) AR="${i#AR=}" ;;
        ARCH=*) ARCH="${i#ARCH=}" ;;
        CFLAGS=*) CFLAGS="${i#CFLAGS=}" ;;
        *) printf "Unknown argument: %s\n" "$i" >&2
        exit 1
        ;;
    esac
done

if [ ! "$ARCH" ]; then
    if ! mach="$($CC -dumpmachine)"; then
        printf "Could not query CC's machine type\n" >&2
        exit 1
    fi
    case "$mach" in
        i?86-*) ARCH=i386 ;;
        x32-*) ARCH=x32 ;;
        x86_64-*) ARCH=x86_64 ;;
        powerpc64*|ppc64*) ARCH=powerpc64 ;;
        powerpc*|ppc*) ARCH=powerpc ;;
        arm64*|aarch64*) ARCH=aarch64 ;;
        *) printf "Unknown machine type %s\n" "$mach" >&2
        exit 1
        ;;
    esac
fi

if [ ! -d "$srcdir/arch/$ARCH" ]; then
    printf "Unknown arch %s\n" "$ARCH" >&2
    exit 1
fi

set -C
i=1
tmpc=tmpc-$$-$i.c
while ! printf "\n" > "$tmpc"
do
    i=$((i+1))
    tmpc=tmpc-$$-$i.c
done
set +C

trap 'rm -f $tmpc' INT QUIT TERM EXIT
trycpp() {
    printf "Testing %s... " "$1"
    printf "#if !(%s)\n#error fail\n#endif" "$2" > $tmpc
    if $CC $CFLAGS -c -o /dev/null $tmpc >/dev/null 2>&1
    then
        printf "yes\n"
        return 0
    else
        printf "no\n"
        return 1
    fi
}

[ "$CFLAGS" ] || CFLAGS="-O3"
if [ "$ARCH" = x86_64 ] && trycpp "if we are actually ILP32" "__ILP32__"
then
    ARCH=x32
fi
flavor=unknown
printf "Trying to determine compiler flavor... "
case "$($CC -v 2>&1 )" in
    *gcc\ version*) flavor=gcc ;;
    *clang\ version*) flavor=clang ;;
esac
printf "%s\n" "$flavor"

pic_default=
if trycpp "if PIC is default" "defined __PIC__ && !defined __PIE__"
then
    pic_default=yes
fi

CFLAGS="$CFLAGS -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-maybe-uninitialized -Wno-abi -Wno-unknown-pragmas -Wno-unused-but-set-variable"

CFLAGS="$CFLAGS -std=c11 -ffreestanding -ffunction-sections -fdata-sections -D_XOPEN_SOURCE -nostdinc -isystem $srcdir/include -isystem $srcdir/arch/$ARCH -isystem obj/include -I $srcdir/src/include"


SRC=
for i in $srcdir/src/*/$ARCH/*.[csS]; do
    [ -e "$i" ] && SRC="$SRC $i"
done

for dir in $srcdir/src/*; do
    for i in $dir/*.c; do
        if [ -e "$i" ]; then
            bn=${i#$dir/}
            bn=${bn%.c}
            [ -e $dir/$ARCH/$bn.[csS] ] || SRC="$SRC $i"
        fi
    done
done

map_obj_file() {
    base=${1#$srcdir/}
    base=${base#src/}
    echo "obj/${base%.[csS]}.o"
}

OBJ=
for i in $SRC; do
    OBJ="$OBJ $(map_obj_file "$i")"
done

if [ "$pic_default" ]; then
    LOBJ="$OBJ"
else
    LOBJ=
    for i in $SRC; do
        ext="${i##*.}"
        o=$(map_obj_file "$i")
        if [ "$ext" = c ]; then
            o="${o%.o}.lo"
        fi
        LOBJ="$LOBJ $o"
    done
fi

DIRS=
for i in $OBJ; do
    DIRS="$DIRS\n${i%/*.o}"
done

DIRS=$(echo "$DIRS" | sort -u)

if [ "$pic_default" ]; then
    CRT1PIC=obj/crt1c.o
else
    CRT1PIC=obj/crt1c.lo
fi

cat >build.ninja <<EOF
cc = $CC
ar = $AR
cflags = $CFLAGS

rule cc
    command = \$cc \$cflags -MD -MF \$out.d -c \$in -o \$out
    depfile = \$out.d

rule ccas
    command = \$cc \$cflags -Wa,--noexecstack -D__ASSEMBLER__ -MD -MF \$out.d -c \$in -o \$out
    depfile = \$out.d

rule as
    command = \$cc -Wa,--noexecstack -c \$in -o \$out

rule ldr
    command = \$cc -r -o \$out \$in

rule lds
    command = \$cc -shared -Wl,-e,_start,-z,defs,--exclude-libs,ALL,--dynamic-list=$srcdir/dynamic.list -o \$out \$in

rule ar
    command = \$ar crs \$out \$in

rule md
    command = mkdir -p \$out

rule mkalltypes
    command = sed -f $srcdir/tools/mkalltypes.sed \$in > \$out

build lib: md
build obj: md
build obj/include: md
build lib/libc.a: ar $OBJ || lib
build lib/rcrt1.o: ldr $CRT1PIC obj/rcrt1s.o || lib
build lib/crt1.o: ldr obj/crt1c.o obj/crt1s.o || lib
build lib/Scrt1.o: ldr $CRT1PIC obj/crt1s.o || lib
build lib/crti.o: as $srcdir/crt/crti.s
build lib/crtn.o: as $srcdir/crt/crtn.s
build obj/crt1c.o: cc $srcdir/crt/crt1c.c || obj
build obj/rcrt1s.o: ccas $srcdir/crt/$ARCH/rcrt1s.S || obj
build obj/include/alltypes.h: mkalltypes $srcdir/arch/$ARCH/alltypes.h.in $srcdir/include/alltypes.h.in || obj/include
EOF

if [ "$pic_default" ]; then
    echo "build lib/libc.so: lds obj/rcrt1s.o obj/ldso.o $OBJ || lib" >>build.ninja
    echo "build obj/ldso.o: cc $srcdir/ldso/ldso.c | obj/include/alltypes.h || obj" >>build.ninja
else
    cat >>build.ninja <<EOF
rule ccpic
    command = \$cc \$cflags -MD -MF \$out.d -c -fPIC \$in -o \$out
    depfile = \$out.d

build obj/crt1c.lo: ccpic $srcdir/crt/crt1c.c || obj
build lib/libc.so: lds obj/rcrt1s.o obj/ldso.lo $LOBJ || lib
build obj/ldso.lo: ccpic $srcdir/ldso/ldso.c | obj/include/alltypes.h || obj
EOF
fi

if [ -e $srcdir/crt/$ARCH/crt1s.S ]
then
    echo "build obj/crt1s.o: ccas $srcdir/crt/$ARCH/crt1s.S || obj" >>build.ninja
else
    echo "build obj/crt1s.o: as $srcdir/crt/$ARCH/crt1s.s || obj" >>build.ninja
fi

for i in $DIRS; do
    echo "build $i: md" >> build.ninja
done

for i in $SRC; do
    extra=""
    case "$i" in
        *.c) builder=cc; extra="| obj/include/alltypes.h" ;;
        *.S) builder=ccas ;;
        *.s) builder=as ;;
    esac
    o=$(map_obj_file "$i")
    dir=${o%/*.o}
    echo "build $o: $builder $i $extra || $dir" >> build.ninja
    if ! [ "$pic_default" ] && [ "$builder" = cc ]; then
        echo "build ${o%.o}.lo: ccpic $i $extra || $dir" >> build.ninja
    fi
done
