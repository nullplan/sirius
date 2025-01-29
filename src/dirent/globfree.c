#include <glob.h>
#include <stdlib.h>
void globfree(glob_t *g)
{
    for (size_t i = 0; i < g->gl_pathc; i++)
        free(g->gl_pathv[g->gl_offs + i]);
    free(g->gl_pathv);
}
