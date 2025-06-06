#!/usr/bin/env python3

import os
import os.path
import sys
import subprocess
import fnmatch
import tempfile
import atexit
import glob
import signal

tmpc = ""
cc = []
cflags = []
srcdir = ""

def cleanup(signum, frame):
    os.unlink(tmpc)
    signal.signal(signum, SIG_DFL)
    signal.raise_signal(signum)

def trycpp(text, condition):
    print("Testing %s... " % text, end='')
    with open(tmpc, mode='w') as f:
        print("#if !(%s)\n#error fail\n#endif" % condition, file=f)
    if subprocess.run(cc + cflags + ["-c", tmpc, "-o", "/dev/null"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0:
        print("yes")
        return True
    else:
        print("no")
        return False

def trycc(text, source):
    print("Testing %s... " % text, end='')
    with open(tmpc, mode='w') as f:
        print(source, file=f)
    if subprocess.run(cc + cflags + ["-c", tmpc, "-o", "/dev/null"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0:
        print("yes")
        return True
    else:
        print("no")
        return False

def map_obj_file(name):
    name = name.removeprefix(srcdir + "/")
    name = name.removeprefix("src/")
    name = name[:-2]
    return "obj/" + name + ".o"

def map_lib_obj_file(name):
    oname = map_obj_file(name)
    if name[-2:] == ".c": oname = oname[:-2] + ".lo"
    return oname
    
def find_src(base, arch):
    src = glob.glob(f"{base}/{arch}/*.[csS]")
    for directory in glob.glob(base):
        for file in glob.glob(directory + "/*.c"):
            bn=os.path.basename(file)[:-2]
            abn = directory + "/" + arch + "/" + bn
            if not os.path.exists(abn + ".c") and not os.path.exists(abn + ".s") and not os.path.exists(abn + ".S"):
                src += [ file ]
    return src


if __name__ == "__main__":
    os.environ['LC_ALL'] = 'C'
    cc = os.getenv("CC", default="").split()
    ar = os.getenv("AR")
    arch = os.getenv("ARCH")
    cflags = os.getenv("CFLAGS", default="").split()
    do_static = True
    do_shared = True

    if cc == []: cc = ["cc"]
    if ar == None: ar = "ar"

    srcdir = os.path.dirname(sys.argv[0])
    if srcdir == "": srcdir="."
    for i in range(1, len(sys.argv)):
        arg = sys.argv[i]
        if arg[:3] == "CC=": cc = arg[3:].split()
        elif arg[:3] == "AR=": ar = arg[3:]
        elif arg[:5] == "ARCH=": arch = arg[5:]
        elif arg[:7] == "CFLAGS=": cflags = arg[7:].split()
        elif arg == "--disable-static": do_static = False
        elif arg == "--disable-shared": do_shared = False
        else:
            print("Unknown argument: %s" % arg, file=sys.stderr)
            sys.exit(1)

    if not do_static and not do_shared:
        print("Nothing to build.", file = sys.stderr)
        sys.exit(1)

    if arch == None or len(arch) == 0:
        machine = subprocess.check_output(cc + ["-dumpmachine"], encoding="utf-8")
        if fnmatch.fnmatch(machine, "i?86-*"): arch = "i386"
        elif fnmatch.fnmatch(machine, "x32-*"): arch = "x32"
        elif fnmatch.fnmatch(machine, "x86_64-*"): arch = "x86_64"
        elif fnmatch.fnmatch(machine, "powerpc64*") or fnmatch.fnmatch(machine, "ppc64*"): arch = "powerpc64"
        elif fnmatch.fnmatch(machine, "powerpc*") or fnmatch.fnmatch(machine, "ppc*"): arch = "powerpc"
        elif fnmatch.fnmatch(machine, "arm64*") or fnmatch.fnmatch(machine, "aarch64*"): arch = "aarch64"
        else:
            print("Unknown machine type: %s" % machine, file=sys.stderr)
            sys.exit(1)
    if not os.path.isdir(srcdir + "/arch/" + arch):
        print("Unknown architecture: %s" % arch, file=sys.stderr)
        sys.exit(1)

    (file, tmpc) = tempfile.mkstemp(suffix=".c", dir=".")
    os.close(file)

    atexit.register(os.unlink, tmpc)
    signal.signal(signal.SIGINT, cleanup)
    signal.signal(signal.SIGQUIT, cleanup)
    signal.signal(signal.SIGTERM, cleanup)

    if cflags == []: cflags = ["-O3"]
    if arch == "x86_64" and trycpp("if we are actually ILP32", "__ILP32__"): arch = "x32"
    if arch == "powerpc64":
        if not trycpp("if correct ABI is in use", "_CALL_ELF==2"): sys.exit(1)

    if arch == "powerpc64" or arch == "powerpc":
        if trycpp("if unsupported long double representation is used", "__LDBL_MANT_DIG__==106"): sys.exit(1)

    if arch == "i386":
        if not trycc("if bp can be used in asm", 'int foo(int x) { register int y __asm__("ebp") = x; __asm__ ("" :: "r"(y)); }'):
            cflags += ["-DBROKEN_EBP_ASM"]


    flavor = "unknown"
    print("Trying to determine compiler flavor... ", end = '')
    version = subprocess.check_output(cc + ["-v"], stderr=subprocess.STDOUT, encoding="utf-8")
    if "gcc version" in version: flavor = "gcc"
    elif "clang version" in version: flavor = "clang"
    print(flavor)

    pic_default = trycpp("if PIC is default", "__PIC__")

    # XXX: Taylor warning flags to CC flavor sensibly
    cflags += ["-Wall","-Wextra", "-Wno-sign-compare", "-Wno-unused-parameter", "-Wno-maybe-uninitialized", "-Wno-abi", "-Wno-unknown-pragmas", "-Wno-unused-but-set-variable"]

    cflags += ["-std=c11", "-ffreestanding", "-ffunction-sections", "-fdata-sections", "-D_XOPEN_SOURCE", "-nostdinc", "-isystem", f"{srcdir}/include", "-isystem", f"{srcdir}/arch/{arch}", "-isystem", "obj/include", "-I", f"{srcdir}/src/include"]

    src = find_src(srcdir + "/src/*", arch)
    obj = [map_obj_file(i) for i in src]
    libsrc = []
    libobj = []
    if do_shared:
        libsrc = find_src(srcdir + "/ldso", arch)
        if pic_default: libobj = [map_obj_file(i) for i in libsrc + src]
        else: libobj = [map_lib_obj_file(i) for i in libsrc + src]
    dirs = list(dict.fromkeys(map(os.path.dirname, obj + libobj)))

    if pic_default:
        crt1pic = "obj/crt1c.o"
        cflags += [ "-fPIC" ]
    else:
        crt1pic = "obj/crt1c.lo"

    libgcc = subprocess.check_output(cc + ["-print-libgcc-file-name"], encoding="utf-8").strip()
    with open("build.ninja", mode="w") as f:
        f.write(f'''cc = {' '.join(cc)}
ar = {ar}
cflags = {' '.join(cflags)}

rule cc
    command = $cc $cflags -MD -MF $out.d -c $in -o $out
    depfile = $out.d

rule ccas
    command = $cc $cflags -Wa,--noexecstack -D__ASSEMBLER__ -MD -MF $out.d -c $in -o $out
    depfile = $out.d

rule as
    command = $cc -Wa,--noexecstack -c $in -o $out

rule ldr
    command = $cc -r -o $out $in

rule lds
    command = $cc -nostdlib -shared -Wl,-z,defs,--exclude-libs,ALL,--dynamic-list={srcdir}/dynamic.list,--hash-style=both,--gc-sections -o $out $in {libgcc}

rule ar
    command = $ar crs $out $in

rule md
    command = mkdir -p $out

rule mkalltypes
    command = sed -f {srcdir}/tools/mkalltypes.sed $in > $out

build lib: md
build obj: md
build obj/include: md
build lib/crti.o: as {srcdir}/crt/crti.s
build lib/crtn.o: as {srcdir}/crt/crtn.s
build obj/include/alltypes.h: mkalltypes {srcdir}/arch/{arch}/alltypes.h.in {srcdir}/include/alltypes.h.in || obj/include
build obj/rcrt1s.o: ccas {srcdir}/crt/{arch}/rcrt1s.S || obj
''')
        if do_static:
            f.write(f'''
build lib/libc.a: ar {' '.join(obj)} || lib
build lib/crt1.o: ldr obj/crt1c.o obj/crt1s.o || lib
build lib/rcrt1.o: ldr {crt1pic} obj/rcrt1s.o || lib
''')
        if not pic_default: f.write(f'''rule ccpic
    command = $cc $cflags -MD -MF $out.d -c -fPIC $in -o $out
    depfile = $out.d

build obj/crt1c.lo: ccpic {srcdir}/crt/crt1c.c || obj\n''')
        if do_static or pic_default: f.write(f"build obj/crt1c.o: cc {srcdir}/crt/crt1c.c || obj\n")
        if do_shared:
            f.write(f"build lib/Scrt1.o: ldr {crt1pic} obj/crt1s.o || lib\n")
            if pic_default:
                f.write(f"build lib/libc.so: lds obj/rcrt1s.o {' '.join(libobj)} || lib\n")
            else:
                f.write(f"build lib/libc.so: lds obj/rcrt1s.o {' '.join(libobj)} || lib\n")
        if os.path.exists(f"{srcdir}/crt/{arch}/crt1s.S"):
            f.write(f"build obj/crt1s.o: ccas {srcdir}/crt/{arch}/crt1s.S || obj\n")
        else:
            f.write(f"build obj/crt1s.o: as {srcdir}/crt/{arch}/crt1s.s || obj\n")

        for i in dirs: f.write(f"build {i}: md\n")
        for i in src:
            o = map_obj_file(i)
            d = os.path.dirname(o)
            if i.endswith(".c"):
                f.write(f"build {o}: cc {i} | obj/include/alltypes.h || {d}\n")
                if do_shared and not pic_default:
                    f.write(f"build {o[:-2]}.lo: ccpic {i} | obj/include/alltypes.h || {d}\n")
            elif i.endswith(".S"): f.write(f"build {o}: ccas {i} || {d}\n")
            elif i.endswith(".s"): f.write(f"build {o}: as {i} || {d}\n")

        for i in libsrc:
            o = map_obj_file(i) if pic_default else map_lib_obj_file(i)
            d = os.path.dirname(o)
            if i.endswith(".c"):
                f.write(f"build {o}: {'cc' if pic_default else 'ccpic'} {i} | obj/include/alltypes.h || {d}\n")
            elif i.endswith(".S"): f.write(f"build {o}: ccas {i} || {d}\n")
            elif i.endswith(".s"): f.write(f"build {o}: as {i} || {d}\n")
