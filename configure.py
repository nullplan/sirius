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

def trycpp(text, condition):
    return trycc(text, "#if !(%s)\n#error fail\n#endif" % condition)

ldflags_try=[]
def tryldflag(flag, lst):
    print("Testing if linker supports %s... " % flag, end='')
    with open(tmpc, mode='w') as f:
        print("typedef int foo;", file=f)
    if subprocess.run(cc + ldflags_try + [flag] + ["-nostdlib","-shared","-o","/dev/null", tmpc], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0:
        print("yes")
        lst += [flag]
        return True
    else:
        print("no")
        return False

cflags_try=[]
def tryccflag(flag, lst):
    print("Testing if compiler supports %s... " % flag, end='')
    with open(tmpc, mode='w') as f:
        print("typedef int foo;", file=f)
    if subprocess.run(cc + cflags_try + [flag] + ["-c","-o","/dev/null",tmpc], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0:
        print("yes")
        lst += [flag]
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
    cc = os.getenv("CC", default="cc").split()
    ar = os.getenv("AR", default="ar")
    arch = os.getenv("ARCH")
    cflags = os.getenv("CFLAGS", default="-O3").split()
    do_static = True
    do_shared = True

    srcdir = os.path.dirname(sys.argv[0])
    if srcdir == "": srcdir="."
    for arg in sys.argv[1:]:
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

    if arch == "x86_64" and trycpp("if we are actually ILP32", "__ILP32__"): arch = "x32"
    if arch == "powerpc64":
        if not trycpp("if correct ABI is in use", "_CALL_ELF==2"): sys.exit(1)

    if arch == "powerpc64" or arch == "powerpc":
        if trycpp("if unsupported long double representation is used", "__LDBL_MANT_DIG__==106"): sys.exit(1)

    flavor = "unknown"
    print("Trying to determine compiler flavor... ", end = '')
    version = subprocess.check_output(cc + ["-v"], stderr=subprocess.STDOUT, encoding="utf-8")
    if "gcc version" in version: flavor = "gcc"
    elif "clang version" in version: flavor = "clang"
    print(flavor)

    pic_default = trycpp("if PIC is default", "__PIC__")

# first: add options to trial flags to error for unknown flags
    tryccflag("-Werror=unknown-warning-option", cflags_try)
    tryccflag("-Werror=unused-command-line-argument", cflags_try)
    tryccflag("-Werror=ignored-optimization-argument", cflags_try)
    tryldflag("-Werror=unknown-warning-option", ldflags_try)
    tryldflag("-Werror=unused-command-line-argument", ldflags_try)

    # now probe LDFLAGS:
    ldflags = []
    # sort input sections by alignment to reduce amount of padding
    tryldflag("-Wl,--sort-section,alignment", ldflags)
    tryldflag("-Wl,--sort-common", ldflags)

    # remove unreferenced input sections. This also gets rid of some
    # weak references we would otherwise have to deal with
    tryldflag("-Wl,--gc-section", ldflags)

    # there are too many stories of tools needing a SysV hash table
    tryldflag("-Wl,--hash-style=both", ldflags)

    # undefined references are normally allowed in shared libs, but
    # won't work for libc
    tryldflag("-Wl,-z,defs", ldflags)

    # we are not in the business of exporting compiler internals, so don't.
    tryldflag("-Wl,--exclude-libs,ALL", ldflags)

    # Link every internal reference internally, except for the symbols on the
    # list. Global data symbols must remain overridable to support copy relocs.
    # I have also added malloc &c. to the list to allow malloc replacement libs
    # to work.
    tryldflag(f"-Wl,--dynamic-list={srcdir}/dynamic.list", ldflags)

    # I would also very much like to exercise the DT_RELR code, please. And
    # this flag has the side effect of combining GOT relocations for the same
    # symbol.
    tryldflag("-Wl,-z,pack-relative-relocs", ldflags)

    # sirius does not support lazy linking, so make the linker take
    # advantage
    tryldflag("-Wl,-z,now", ldflags)
    tryldflag("-Wl,-z,relro", ldflags)

    # sirius is written in C11
    tryccflag("-std=c11", cflags)
    # get rid of system includes
    tryccflag("-nostdinc", cflags)
    # a libc must always be compiled in freestanding mode, else all the
    # identifiers are reserved
    if not tryccflag("-ffreestanding", cflags): tryccflag("-fno-builtin", cflags)
    # try to get sane excess precision behavior.
    # i386 is the only platform with excess precision, anyway
    if not tryccflag("-fexcess-precision=standard", cflags) and arch == "i386":
        tryccflag("-ffloat-store", cflags)
    # I want to insist my code conforms to strict aliasing rules. However,
    # since the option is normally turned off, turning it on is not well tested,
    # and there are likely nasty to track bugs present. So for the sake of sanity:
    tryccflag("-fno-strict-aliasing", cflags)
    # Rather than clutter all the asm files with stack annotation sections, we
    # can just add a command line option that has the same effect
    tryccflag("-Wa,--noexecstack", cflags)
    # I would very much like to use -pipe always.
    tryccflag("-pipe", cflags)
    # Unwinding through C code is not defined. Rather than waste processing
    # power and space, better to not emit these tables at all
    tryccflag("-fno-unwind-tables", cflags)
    tryccflag("-fno-asynchronous-unwind-tables", cflags)
    # These make --gc-sections work better. Also apparently may prevent
    # compiler bugs from biting me.
    tryccflag("-ffunction-sections", cflags)
    tryccflag("-fdata-sections", cflags)

    # on i386, set default -march to i486, because i386 doesn't have cmpxchg,
    # and is therefore not supportable.
    if arch == "i386":
        if not any(flg.startswith("-march=") for flg in cc + cflags):
            tryldflag("-march=i486", cflags)
        if not any(flg.startswith("-mtune=") for flg in cc + cflags):
            tryldflag("-mtune=generic", cflags)

    # warning options:
    # on clang, some warnings are enabled by default, but -w rids me of them
    # on GCC, -w disables all warnings forever.
    if flavor == "clang": tryccflag("-w", cflags)
    # these are generally useful/necessary to have
    tryccflag("-Werror=implicit-function-declaration", cflags) # especially this one
    tryccflag("-Werror=implicit-int", cflags)
    tryccflag("-Werror=pointer-sign", cflags)
    tryccflag("-Werror=pointer-arith", cflags)
    tryccflag("-Werror=int-conversion", cflags)
    tryccflag("-Werror=incompatible-pointer-types", cflags)
    tryccflag("-Werror=discarded-qualifiers", cflags)
    tryccflag("-Werror=discarded-array-qualifiers", cflags)
    # GCC is really getting on my nerves with this one though:
    if flavor == "gcc": tryccflag("-Wno-abi", cflags)

    # Clang warns about LDFLAGS in the compilation step.
    # But user's CFLAGS may include linker flags, so this
    # shuts up the compiler
    if flavor == "clang": tryccflag("-Qunused-arguments", cflags)

    # these are the useful warnings I enable
    tryccflag("-Waddress", cflags)
    tryccflag("-Warray-bounds", cflags)
    tryccflag("-Wchar-subscripts", cflags)
    tryccflag("-Wduplicate-decl-specifier", cflags)
    tryccflag("-Winit-self", cflags)
    tryccflag("-Wreturn-type", cflags)
    tryccflag("-Wsequence-point", cflags)
    tryccflag("-Wstrict-aliasing", cflags)
    tryccflag("-Wunused-function", cflags)
    tryccflag("-Wunused-label", cflags)
    tryccflag("-Wunused-variable", cflags)
    tryccflag("-Wimplicit-fallthrough", cflags)

    cflags += ["-D_XOPEN_SOURCE", "-isystem", f"{srcdir}/include", "-isystem", f"{srcdir}/arch/{arch}", "-isystem", "obj/include", "-I", f"{srcdir}/src/include"]

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

    # try to find the compiler support library
    libgcc = []
    if tryldflag("-lgcc", libgcc): tryldflag("-lgcc_eh", libgcc)    # libgcc for GCC
    if not libgcc: tryldflag("-lcompiler_rt", libgcc)               # compiler_rt for clang
    if not libgcc:
        # if that didn't work, try again with -print-libgcc-file-name
        libgcc_fn = subprocess.check_output(cc + ["-print-libgcc-file-name"], encoding="utf-8").strip()
        tryldflag(libgcc_fn, libgcc)
    # else we have to hope the compiler doesn't need any library

    with open("build.ninja", mode="w") as f:
        f.write(f'''cc = {' '.join(cc)}
ar = {ar}
cflags = {' '.join(cflags)}

rule cc
    command = $cc $cflags -MD -MF $out.d -c $in -o $out
    depfile = $out.d

rule as
    command = $cc $cflags -c $in -o $out

rule ldr
    command = $cc -r -o $out $in

rule lds
    command = $cc $cflags -nostdlib -shared {' '.join(ldflags)} -o $out $in {' '.join(libgcc)}

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
build obj/rcrt1s.o: cc {srcdir}/crt/{arch}/rcrt1s.S || obj
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
            f.write(f"build lib/libc.so: lds obj/rcrt1s.o {' '.join(libobj)} || lib\n")
        if os.path.exists(f"{srcdir}/crt/{arch}/crt1s.S"):
            f.write(f"build obj/crt1s.o: cc {srcdir}/crt/{arch}/crt1s.S || obj\n")
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
            elif i.endswith(".S"): f.write(f"build {o}: cc {i} || {d}\n")
            elif i.endswith(".s"): f.write(f"build {o}: as {i} || {d}\n")

        for i in libsrc:
            o = map_obj_file(i) if pic_default else map_lib_obj_file(i)
            d = os.path.dirname(o)
            if i.endswith(".c"):
                f.write(f"build {o}: {'cc' if pic_default else 'ccpic'} {i} | obj/include/alltypes.h || {d}\n")
            elif i.endswith(".S"): f.write(f"build {o}: cc {i} || {d}\n")
            elif i.endswith(".s"): f.write(f"build {o}: as {i} || {d}\n")
