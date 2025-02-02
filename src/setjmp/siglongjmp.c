#include <setjmp.h>

_Noreturn void siglongjmp(sigjmp_buf jb, int val)
{
    longjmp(jb->__jb, val);
}
