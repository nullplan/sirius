#ifndef CRYPT_H
#define CRYPT_H

#include <unistd.h>
#define CRYPT_LEN       256
extern hidden char *__crypt_r(const char *, const char *, char *);
extern hidden char *__md5crypt(const char *, const char *, char *);
extern hidden char *__sha256crypt(const char *, const char *, char *);
extern hidden char *__sha512crypt(const char *, const char *, char *);
extern hidden char *__yescrypt(const char *, const char *, char *);
extern hidden char *__gyescrypt(const char *, const char *, char *);
extern hidden char *__bcrypt(const char *, const char *, char *);
#endif
