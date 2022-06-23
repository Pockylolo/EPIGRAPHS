#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/////////////***FUNCTIONS***///////////////////
int len_list(struct list *l)
{
    int i = 0;

    if (l == NULL)
        return 0;

    while(l->next != NULL)
    {
        l = l->next;
        i++;
    }
    return i;
}

struct list *list_init()
{
    struct list *list = malloc(sizeof(struct list));
    list->elmt = NULL;
    list->next = NULL;
    return list;
}

void free_list(struct list *l)
{
    struct list *p = l;
    while(p != NULL)
    {
        struct list *tmp = p->next;
        free(p);
        p = tmp;
    }
}

int list_is_empty(struct list *l)
{
    if (l->next == NULL)
        return 1;
    return 0;
}

void push_list_front(struct list *l, void *elm)
{
    struct list *elm_list = list_init();
    elm_list->elmt = elm;
    elm_list->next = l->next;
    l->next = elm_list;
}
