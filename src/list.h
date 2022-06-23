#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

//////////***STRUCT OF A DYNAMIC LIST***//////////
struct list
{
    void *elmt;
    struct list *next;
};

/////////***FUNCTIONS***/////////////////////////

int len_list(struct list *l);
struct list *list_init();
void free_list(struct list *l);
void push_list_front(struct list *l, void *elm);

#endif
