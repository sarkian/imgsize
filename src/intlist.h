#ifndef INTLIST_H
#define INTLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


typedef struct {
    size_t size;
    size_t rsize;
    int * data;
} intlist;


intlist *   intlist_init(void);
intlist *   intlist_init_size(size_t);
intlist *   intlist_init_args(size_t, ...);
void        intlist_push(intlist *, int);
int *       intlist_get(intlist *, size_t);
size_t *    intlist_search(intlist *, int);
void        intlist_free(intlist **);

#endif

/*
 * vim:ft=c:
 */
