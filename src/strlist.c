#include <strlist.h>

strlist * strlist_init()
{
    return strlist_init_size(8);
}


strlist * strlist_init_size(size_t size)
{
    strlist * lst = malloc(sizeof(strlist *));
    lst->size = 0;
    lst->rsize = size;
    lst->data = malloc(sizeof(char *) * lst->rsize);
    return lst;
}


strlist * strlist_init_args(size_t n, ...)
{
    va_list ap;
    char * tmp;
    size_t i = 0;
    strlist * lst = strlist_init_size(n);
    va_start(ap, n);
    while(i < n) {
        strlist_push(lst, va_arg(ap, char *));
        i++;
    }
    va_end(ap);
    return lst;
}


void strlist_push(strlist * lst, char * item)
{
    size_t nsize;
    size_t nrsize;
    char ** buff;
    nsize = lst->size + 1;
    if(nsize > lst->rsize) {
        nrsize = lst->rsize * 2;
        buff = malloc(sizeof(char *) * nrsize);
        memcpy(buff, lst->data, sizeof(char *) * lst->rsize);
        free(lst->data);
        lst->data = buff;
        lst->rsize = nrsize;
    }
    lst->data[lst->size] = malloc(sizeof(char *) * strlen(item));
    strcpy(lst->data[lst->size], item);
    lst->size = nsize;
}


char * strlist_get(strlist * lst, size_t index)
{
    if(lst->size > 0 && index < lst->size)
        return lst->data[index];
    else
        return NULL;
}


size_t * strlist_search(strlist * lst, char * needle)
{
    size_t * n = malloc(sizeof(size_t));
    *n = 0;
    while(*n < lst->size) {
        if(strcmp(lst->data[*n], needle) == 0)
            return n;
        *n += 1;
    }
    return NULL;
}


void strlist_free(strlist ** lst)
{
    size_t i = 0;
    while(i < (*lst)->size) {
        free((*lst)->data[i]);
        i++;
    }
    free((*lst)->data);
    free(*lst);
    *lst = NULL;
}


