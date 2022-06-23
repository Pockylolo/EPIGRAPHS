#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "graph.h"
#include <string.h>

void save_graph(struct graph *G, const char *filepath)
{
    struct list *p = G->nodes;
    FILE *fp;
    int x=0;
    x=G->oriented;
    
    fp= fopen(filepath,"w");
    if (fp == NULL)
        printf("ne souvre pas\n");

    fprintf(fp,"%d\n",x);

    for (int i = 0; i < G->order; i++)
    {
        p = p->next;
        struct node *n = p->elmt;

        struct list *p2 = n->adj; 

        int len = len_list(p2);

        for (int j = 0; j < len; j++) 
        {
            p2 = p2->next; 
            struct node *adj = p2->elmt;
            fprintf(fp,"%d  %d    ", i, adj->data);
        }
        if(len==0)
        {
            fprintf(fp,"%d",i); //sommet isolé;
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}
struct graph *create_file(const char *filepath)
{
   /*
int opp;*/
    int hb=50;
    //nature of graph
    /*fscanf(nat, "%[^\n ] ",&opp);
    hb= opp-'0';*/

    FILE *file=fopen(filepath,"r");
    char buffer[512];
    char separator[]=" ";
    char* p;
    char* val1;
    char* val2;
    int i=0;
    struct list *zebi = list_init();
    if (fgets(buffer,512,file) == NULL)
        return NULL;
    p=strtok(buffer,separator);
    hb=atoi(p);

    while(fgets(buffer,512,file)!=NULL)
    {
        p=strtok(buffer,separator);
        while(p!=NULL)
        {
            val1=p;
            if(i<atoi(val1))
            {
                i=atoi(val1);
            }
            p=strtok(NULL,separator);
            if(p!=NULL)
            {
                val2=p;
                if(i<atoi(val2))
                {
                    i=atoi(val2);
                }
                if(hb == 0) //non-oriented
                {
                    if (atoi(val1) < atoi(val2))
                    {
                    struct edge *e1 = new_edge(atoi(val1), atoi(val2));
                push_list_front(zebi, (void *)e1);
                p=strtok(NULL,separator);
                    }
                    continue;
                }

                struct edge *e1 = new_edge(atoi(val1), atoi(val2));
                push_list_front(zebi, (void *)e1);
                p=strtok(NULL,separator);
                continue;

            }
            //sommet isolé
            /*struct node *e2=new_node(atoi(val1));
            push_list_front(zebi,(void*)e2);*/
        }
    }
    fclose(file);
    struct graph *G = create_graph(hb, i+1, zebi);
    return G;
}

/*int main(void)
{
    struct graph* G =malloc(sizeof(struct graph));
    *G=create_file("src/test.txt");
    print_graph(G);
    return 1;
}*/
