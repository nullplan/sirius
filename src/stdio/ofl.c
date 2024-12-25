#include <stdio.h>

static FILE *head;

static FILE *null = 0;
weak_alias(__stdin_used, null);
weak_alias(__stdout_used, null);
weak_alias(__stderr_used, null);

static void add_file(FILE *f)
{
    f->next = head;
    if (head) head->prev = f;
    head = f;
}

hidden void __stdio_list_init(void)
{
    if (__stdin_used) add_file(__stdin_used);
    if (__stdout_used) add_file(__stdout_used);
    if (__stderr_used) add_file(__stderr_used);
}

hidden FILE *__ofl_lock(void)
{
    return head;
}

hidden void __ofl_unlock(FILE *h)
{
    head = h;
}

hidden void __stdio_exit(void)
{
    for (FILE *f = __ofl_lock(); f; f = f->next)
    {
        if (IS_READ(f) && f->pos != f->end) f->seek(f, f->end - f->pos, SEEK_CUR);
        if (IS_WRITE(f) && f->pos != f->buf) f->write(f, 0, 0);
    }
}
weak_alias(__stdio_exit_needed, __stdio_exit);
