#include <features.h>
extern _Noreturn void __start_libc(int (*)(int, char **, char **), long *);
extern int main(int, char **, char **);

hidden _Noreturn void _start_c(long *sp)
{
  __start_libc(main, sp);
}
