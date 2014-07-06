#include <intlist.h>


intlist * intlist_init()
{
    intlist_init_size(8);
}


intlist * intlist_init_size(size_t size)
{
    intlist * lst = malloc(sizeof(intlist *));
    lst->size = 0;
    lst->rsize = size;
    lst->data = malloc(sizeof(int) * lst->rsize);
    return lst;
}


intlist * intlist_init_args(size_t n, ...)
{
    va_list ap;
    size_t i = 0;
    intlist * lst = intlist_init_size(n);
    va_start(ap, n);
    while(i < n) {
        intlist_push(lst, va_arg(ap, int));
        i++;
    }
    va_end(ap);
    return lst;
}


void intlist_push(intlist * lst, int item)
{
    size_t nsize;
    size_t nrsize;
    int * buff;
    nsize = lst->size + 1;
    if(nsize > lst->rsize) {
        nrsize = lst->rsize * 2;
        buff = malloc(sizeof(int) * nrsize);
        memcpy(buff, lst->data, sizeof(int) * lst->rsize);
        free(lst->data);
        lst->data = buff;
        lst->rsize = nrsize;
    }
    lst->data[lst->size] = item;
    lst->size = nsize;
}


int * intlist_get(intlist * lst, size_t index)
{
    int * res;
    if(lst->size > 0 && index < lst->size) {
        res = malloc(sizeof(int));
        *res = lst->data[index];
        return res;
    }
    else
        return NULL;
}


size_t * intlist_search(intlist * lst, int needle)
{
    size_t * n = malloc(sizeof(size_t));
    *n = 0;
    while(*n < lst->size) {
        if(lst->data[*n] == needle)
            return n;
        *n += 1;
    }
    return NULL;
}


void intlist_free(intlist ** lst)
{
    free((*lst)->data);
    free(*lst);
    *lst = NULL;
}

