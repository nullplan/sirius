#include <wordexp.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <spawn.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "libc.h"

static int scan_expr(const char *restrict s, int flags)
{
    return 0; /* XXX: Actually implement this one of these days */
}

static void reap(pid_t pid)
{
    int rv;
    do rv = waitpid(pid, 0, 0);
    while (rv != pid && (rv != -1 || errno != ECHILD));
}

static char *getword(FILE *f)
{
    char *s = 0;
    size_t len = 0;
    size_t alloc = 0;
    int c;
    for (;;) {
        c = fgetc(f);
        if (c == EOF) {
            free(s);
            return 0;
        }
        if (len == alloc) {
            size_t attempt = c == 0? alloc + 1 : alloc? alloc + alloc / 2 : 16;
            char *p = realloc(s, attempt);
            if (!p) {
                free(s);
                return 0;
            }
            s = p;
            alloc = attempt;
        }
        s[len++] = c;
        if (!c) return s;
    }
}
struct stringlist {
    char **words;
    size_t n;
    size_t alloc;
};

static int do_wordexp(const char *restrict s, struct stringlist *list, int flags)
{
    const char *const redir = flags & WRDE_SHOWERR? "" : "2>/dev/null";
    int p[2];
    list->words = 0;
    list->n = 0;
    list->alloc = 0;

    if (pipe2(p, O_CLOEXEC))
        return WRDE_NOSPACE;

    if (p[1] == 1) {
        int tmp = fcntl(p[1], F_DUPFD_CLOEXEC, 3);
        if (tmp == -1) {
            close(p[0]);
            close(p[1]);
            return WRDE_NOSPACE;
        }
        close(p[1]);
        p[1] = tmp;
    }

    posix_spawn_file_actions_t psfa;
    pid_t pid = -1;
    if (posix_spawn_file_actions_init(&psfa) == 0)
    {
        if (posix_spawn_file_actions_adddup2(&psfa, p[1], 1)
                || posix_spawn(&pid, "/bin/sh", &psfa, 0,
                    (char *const []){"sh", "-c", "eval printf \"%s\\\\\\\\0\" x $1 $2", "sh", (char *)s, (char *)redir, 0},
                    __environ))
            pid = -1;
        posix_spawn_file_actions_destroy(&psfa);
    }
    close(p[1]);
    if (pid == -1) {
        close(p[0]);
        return WRDE_NOSPACE;
    }

    FILE *f = fdopen(p[0], "r");
    if (!f) {
        kill(pid, SIGKILL);
        reap(pid);
        close(p[0]);
        return WRDE_NOSPACE;
    }

    free(getword(f));
    if (feof(f)) {
        fclose(f);
        reap(pid);
        return 0; /* empty output means success */
    }

    char *w;

    while ((w = getword(f))) {
        if (list->n == list->alloc) {
            size_t attempt = list->n? list->n + list->n / 2 : 16;
            char **p = realloc(list->words, attempt * sizeof (char *));
            if (!p) {
                fclose(f);
                reap(pid);
                return WRDE_NOSPACE;
            }
            list->words = p;
            list->alloc = attempt;
        }
        list->words[list->n++] = w;
    }
    int rv = feof(f)? 0 : WRDE_NOSPACE;
    if (rv) kill(pid, SIGKILL);
    fclose(f);
    reap(pid);
    return 0;
}

int wordexp(const char *restrict s, wordexp_t *restrict we, int flags)
{
    size_t off = flags & WRDE_DOOFFS? we->we_offs : 0;
    /* simplest implementation of WRDE_REUSE */
    if (flags & WRDE_REUSE) wordfree(we);
    if (!(flags & WRDE_APPEND)) {
        we->we_wordc = 0;
        we->we_wordv = 0;
        we->we_offs = off;
    }

    int rv = scan_expr(s, flags);
    if (rv) return rv;

    struct stringlist list;
    int cs;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    rv = do_wordexp(s, &list, flags);
    pthread_setcancelstate(cs, 0);

    size_t final = off + we->we_wordc + list.n;
    if (final > list.alloc)
    {
        char **p = realloc(list.words, final * sizeof (char *));
        if (!p) {
            for (size_t i = 0; i < list.n; i++)
                free(list.words[i]);
            free(list.words);
            return WRDE_NOSPACE;
        }
        list.words = p;
    }
    memmove(list.words + off + we->we_wordc, list.words, list.n * sizeof (char *));
    memcpy(list.words + off, we->we_wordv, we->we_wordc * sizeof (char *));
    memset(list.words, 0, off * sizeof (char *));
    we->we_wordc += list.n;
    free(we->we_wordv);
    we->we_wordv = list.words;
    return rv;
}

void wordfree(wordexp_t *we)
{
    if (we->we_wordv) {
        for (size_t i = 0; i < we->we_wordc; i++)
            free(we->we_wordv[we->we_offs + i]);
        free(we->we_wordv);
        we->we_wordv = 0;
        we->we_wordc = 0;
    }
}
