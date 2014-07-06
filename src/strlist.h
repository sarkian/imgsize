#ifndef STRLIST_H
#define STRLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


typedef struct {
    size_t size;
    size_t rsize;
    char ** data;
} strlist;


strlist *   strlist_init(void);
strlist *   strlist_init_size(size_t);
strlist *   strlist_init_args(size_t, ...);
void        strlist_push(strlist *, char *);
char *      strlist_get(strlist *, size_t);
size_t *    strlist_search(strlist *, char *);
void        strlist_free(strlist **);

#endif

/*
 * vim:ft=c:
 */
