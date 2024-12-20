#ifndef __STDIO_H
#define __STDIO_H

#include <features.h>
#define __STDC_VERSION_STDIO_H__ 202311L
#define __NEED_NULL
#define __NEED_FILE
#define __NEED_size_t
#define __NEED_va_list
#ifdef _POSIX_C_SOURCE
#define __NEED_ssize_t
#define __NEED_off_t
#endif
#include <alltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct { long long __pos; } fpos_t;
#define _IONBF  0
#define _IOLBF  1
#define _IOFBF  2
#define BUFSIZ  4000
#define EOF (-1)
#define FOPEN_MAX   1024
#define FILENAME_MAX 4096

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define TMP_MAX     10000
#define L_tmpnam    20
#define _PRINTF_NAN_LEN_MAX 4

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

#define stdin (stdin)
#define stdout (stdout)
#define stderr (stderr)

int remove(const char *);
int rename(const char *, const char *);
FILE *tmpfile(void);
char *tmpnam(char *);
int fclose(FILE *);
int fflush(FILE *);
FILE *fopen(const char *__restrict, const char *__restrict);
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict);
void setbuf(FILE *__restrict, char *__restrict);
int setvbuf(FILE *__restrict, char *__restrict, int, size_t);
int fprintf(FILE *__restrict, const char *__restrict, ...);
int fscanf(FILE *__restrict, const char *__restrict, ...);
int printf(const char *__restrict, ...);
int scanf(const char *__restrict, ...);
int snprintf(char *__restrict, size_t, const char *__restrict, ...);
int sprintf(char *__restrict, const char *__restrict, ...);
int sscanf(const char *__restrict, const char *__restrict, ...);
int vfprintf(FILE *__restrict, const char *__restrict, va_list);
int vfscanf(FILE *__restrict, const char *__restrict, va_list);
int vprintf(const char *__restrict, va_list);
int vscanf(const char *__restrict, va_list);
int vsnprintf(char *__restrict, size_t, const char *__restrict, va_list);
int vsprintf(char *__restrict, const char *__restrict, va_list);
int vsscanf(const char *__restrict, const char *__restrict, va_list);
int fgetc(FILE *);
char *fgets(char *__restrict, int, FILE *__restrict);
int fputc(int, FILE *);
int fputs(const char *__restrict, FILE *__restrict);
int getc(FILE *);
int getchar(void);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int ungetc(int, FILE *);
size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
void rewind(FILE *);
void clearerr(FILE *);
int feof(FILE *);
int ferror(FILE *);
void perror(const char *);

#ifdef _POSIX_C_SOURCE
#define L_ctermid 9
char *ctermid(char *);
int dprintf(int, const char *__restrict, ...);
int vdprintf(int, const char *__restrict, va_list);
FILE *fdopen(int, const char *);
int fileno(FILE *);
int fseeko(FILE *, off_t, int);
off_t ftello(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(FILE *);
int putchar_unlocked(FILE *);

ssize_t getline(char **__restrict, size_t *__restrict, FILE *__restrict);
ssize_t getdelim(char **__restrict, size_t *__restrict, int, FILE *__restrict);
FILE *open_memstream(char **, size_t *);
FILE *popen(const char *, const char *);
int pclose(FILE *);
int renameat(int, const char *, int, const char *);
#endif
#ifdef __cplusplus
}
#endif
#endif
