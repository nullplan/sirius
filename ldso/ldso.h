#ifndef LDSO_H
#define LDSO_H

#include <features.h>
#include <sys/types.h>
#include <elf.h>

/* define unmatchable values for internal relocation types here.
 * They get overwritten by reloc_arch.h
 */
enum {
    REL_RELATIVE = -100,
    REL_COPY,
    REL_GOT,
    REL_SYMBOLIC,
    REL_USYMBOLIC,
    REL_PCREL32,
    REL_PLT,
    REL_DTPMOD,
    REL_DTPOFF,
    REL_TPOFF,
    REL_TLSDESC,
};
#include "reloc_arch.h"

#ifndef __SIXTY_FOUR
typedef Elf32_Sym Sym;
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
#define ELFCLASS ELFCLASS32
#define R_SYM(i) ELF32_R_SYM(i)
#define R_TYPE(i) ELF32_R_TYPE(i)
#define ST_TYPE ELF32_ST_TYPE
#define ST_BIND ELF32_ST_BIND
#else
typedef Elf64_Sym Sym;
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
#define ELFCLASS ELFCLASS64
#define R_SYM(i) ELF64_R_SYM(i)
#define R_TYPE(i) ELF64_R_TYPE(i)
#define ST_TYPE ELF64_ST_TYPE
#define ST_BIND ELF64_ST_BIND
#endif


struct dso {
    char *base;
    void *map;
    size_t map_len;
    const Phdr *phdr;
    size_t phnum;
    size_t phent;
    const size_t *dynv;
    dev_t dev;
    ino_t ino;
    const char *name;
    const char *shortname;
    const uint32_t *hashtab;
    const uint32_t *ghashtab;
    const Sym *symtab;
    const char *strtab;
    int relocated;
    int kernel_mapped;
    int tlsid;
    int initialized;
    size_t mark;
    size_t tlsoff;
    struct dso *const *deps;
    struct dso *next, *prev;
    struct dso *fini_next;
    pthread_t initializing_thread;
    uintptr_t relro_start;
    uintptr_t relro_end;
};

struct symdef {
    struct dso *dso;
    const Sym *sym;
};

hidden _Noreturn void __die_early_multi(const char **, size_t);
hidden _Noreturn void __die_early(const char *);
hidden size_t __libc_addend(size_t);
hidden struct dso *__libc_object(void);
hidden _Noreturn void __stage_post_reloc(long *, const size_t *);
hidden void __reclaim_gaps(const struct dso *);
hidden void __reloc_all(struct dso *);
hidden void __relocate(struct dso *);
hidden void __decode_vec(size_t *, const size_t *, size_t);
hidden struct dso *__dl_head(void);
hidden const char *__dl_libpath(void);
hidden void __dl_print_error(const char *, ...);
hidden int __dl_static_tls_cnt(void);
hidden struct symdef __dl_find_sym(const char *, struct dso *, int);
hidden void __process_dynv(struct dso *);
hidden struct dso **__queue_initializers(struct dso *);
hidden int __is_ldd_mode(void);
hidden void __dl_push_back(struct dso *);
hidden void __queue_main_initializers(struct dso *);
hidden void *__map_library(int, struct dso *);
hidden void __load_deps(struct dso *dso, const char *search_path, int at_startup);
hidden void __load_preload(char *preload, const char *search_path);
hidden struct dso *__load_library(const char *name, const char *search_path, int at_startup);
#endif
