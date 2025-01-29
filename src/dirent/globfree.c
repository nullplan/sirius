#include <glob.h>
#include <stdlib.h>
void globfree(glob_t *g)
{
    for (size_t i = 0; i < g->gl_pathc; i++)
        free(g->pathv[g->gl_offs + i]);
    free(g->pathv);
}
