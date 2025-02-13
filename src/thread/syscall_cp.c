#include "syscall.h"

static long dummy(syscall_arg_t a, syscall_arg_t b, syscall_arg_t c, syscall_arg_t d, syscall_arg_t e, syscall_arg_t f, syscall_arg_t nr)
{
    return (__syscall6)(nr, a, b, c, d, e, f);
}
weak_alias(__syscall_cp_c, dummy);

hidden long (__syscall_cp)(syscall_arg_t a, syscall_arg_t b, syscall_arg_t c, syscall_arg_t d, syscall_arg_t e, syscall_arg_t f, syscall_arg_t nr)
{
    return __syscall_cp_c(a, b, c, d, e, f, nr);
}
