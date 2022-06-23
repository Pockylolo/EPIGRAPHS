#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "graph.h"

////////////////////***FUNCTION PART***/////////////////////
struct graph *init_graph(int oriented, int order)
{
    struct graph *G = malloc(sizeof(struct graph));
    //G->nodes = malloc(sizeof(struct list) * order);
    G->nodes = list_init();
    G->oriented = oriented;
    G->order = order;

    return G;
}
///////////////////////////////////////////////////////////
int graph_is_empty(struct graph *G)
{
    if (G->order == 0)
        return 1;

    return 0;
}
///////////////////////////////////////////////////////////
struct node *find_node(struct graph *G, int i)
{
    struct list *p = G->nodes->next;
    while(p != NULL)
    {
        struct node *n = p->elmt;

        if (n->data == i)
            return n;

        p = p->next;
    }

    return NULL;
}
//////////////////////////////////////////////////////////
struct node* new_node(int data)
{
    struct node *node = malloc(sizeof(struct node));
    node->data = data;
    node->adj = list_init();

    return node;
}
//////////////////////////////////////////////////////////
struct edge* new_edge(int src, int dest)
{
    struct edge *edge = malloc(sizeof(struct edge));
    edge->src = src;
    edge->dest = dest;

    return edge;
}
//////////////////////////////////////////////////////////
int edges_equal(int oriented, struct edge *e1, struct edge *e2)
{
    if (e1->src == e2->src)
        if (e1->dest == e2->dest)
            return 1;

    if (oriented == 0)
        if (e1->src == e2->dest)
            if (e1->dest == e2->src)
                return 1;
    return 0;
}
//////////////////////////////////////////////////////////
int is_in_list_edges(int oriented, struct list *l, struct edge *elm)
{
    while(l->next != NULL)
    {
        l = l->next;
        if (edges_equal(oriented, l->elmt, elm))
            return 1;
    }
    return 0;
}
////////////////////////////////////////////////////////
struct list *edges_from_graph(struct graph *G)
{
    struct list *p = G->nodes;
    struct list *edges = list_init();

    if (G->oriented != 1) //non-oriented case
    {
        //look at nodes' adj and optimise looking only when the dest
        //is smaller than source
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
                if (n->data < adj->data)
               {
                    struct edge *e = new_edge(n->data, adj->data);
                    push_list_front(edges, (void *) e);
               }
            }
        }
    }
    else
    {
        //oriented case
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
                struct edge *e = new_edge(n->data, adj->data);
                push_list_front(edges, (void *) e);
            }
        }
    }
    return edges;
}
////////////////////////////////////////////////////////
struct graph* create_graph(int oriented, int order, struct list *edges)
{
    struct graph *G = init_graph(oriented, order);

    if (graph_is_empty(G) == 1)
        return G;

    ////***VERTICES PART***////
    for (int i = order - 1; i >= 0; i--)
    {
        struct node *n = new_node(i);
        push_list_front(G->nodes, (void *)n);
    }

    ////***EDGES PART***////
    int len = len_list(edges);

    if (oriented == 0)      //non-oriented case
    {
        /*WARNING : here the edges list is
         *changed so you have to change here if you have to keep the edges*/
        for (int i = 0; i < len; i++)
        {
            edges = edges->next;
            struct edge *edge = edges->elmt;
            int dest = edge->dest;
            int src = edge->src;

            struct node *node_dest = find_node(G, dest);
            struct node *node_src = find_node(G, src);

            struct node *n_src = new_node(src);
            n_src->adj = node_src->adj;

            struct node *n_dest = new_node(dest);
            n_dest->adj = node_dest->adj;

            push_list_front(node_dest->adj, (void *)n_src);
            push_list_front(node_src->adj, (void *)n_dest);
        }
    }

    if (oriented != 0)      //oriented case
    {
        for (int i = 0; i < len; i++)
        {
            edges = edges->next;
            struct edge *edge = edges->elmt;
            int dest = edge->dest;
            int src = edge->src;

            struct node *node_dest = find_node(G, dest);
            struct node *node_src = find_node(G, src);

            struct node *n_dest = new_node(dest);
            n_dest->adj = node_dest->adj;

            push_list_front(node_src->adj, (void *)n_dest);
        }
    }

    return G;
}
////////////////////////////////////////////////////////
//Return a list of the nodes encountered during a DFS (prefix order)
void _DFS_Prefix(struct node *n, struct graph *G, int *M, int *list, int *k)
{
    M[n->data] = 1;
    int len = len_list(n->adj);
    list[*k] = n->data;              //prefix order
    *k = *k + 1;
    struct list *adj = n->adj->next;

    for (int i = 0; i < len; i++)
    {
        struct node *elm = adj->elmt;
        adj = adj->next;
        if (M[elm->data] == 0)
            _DFS_Prefix(elm, G, M, list, k);
    }
}

int* DFS_Prefix(struct graph *G)
{
    int *M = calloc(G->order, sizeof(int));

    int *list = malloc(sizeof(int) * G->order);
    int *k = malloc(sizeof(int));
    *k = 0;

    struct list *p = G->nodes;
    while(p->next != NULL)
    {
        p = p->next;
        struct node *n = p->elmt;

        if (M[n->data] == 0)
            _DFS_Prefix(n, G, M, list, k);
    }
    free(M);
    free(k);
    return list;
}
////////////////////////////////////////////////////////
//Return a list of the nodes encountered during a DFS (suffix order)
void _DFS_Suffix(struct node *n, struct graph *G, int *M, int *list, int *k)
{
    M[n->data] = 1;
    int len = len_list(n->adj);
    struct list *adj = n->adj->next;

    for (int i = 0; i < len; i++)
    {
        struct node *elm = adj->elmt;
        adj = adj->next;
        if (M[elm->data] == 0)
            _DFS_Suffix(elm, G, M, list, k);
    }
    list[*k] = n->data;
    *k = *k + 1;
}

int* DFS_Suffix(struct graph *G)
{
    int *M = calloc(G->order, sizeof(int));

    int *list = malloc(sizeof(int) * G->order);
    int *k = malloc(sizeof(int));
    *k = 0;

    struct list *p = G->nodes;
    while(p->next != NULL)
    {
        p = p->next;
        struct node *n = p->elmt;

        if (M[n->data] == 0)
            _DFS_Suffix(n, G, M, list, k);
    }
    free(M);
    free(k);
    return list;
}

////////////////////////////////////////////////////////
void print_graph(struct graph *G)
{
    if (graph_is_empty(G) == 1)
        printf("Empty graph\n");

    struct list *p = G->nodes;

    for (int i = 0; i < G->order; i++)
    {
        p = p->next;
        struct node *n = p->elmt;
        printf("%d\t", n->data);

        struct list *p2 = n->adj;

        int len = len_list(p2);
        printf("len_adj = %d\t", len);

        for (int j = 0; j < len; j++)
        {
            p2 = p2->next;
            struct node *adj = p2->elmt;
            printf("(%d -> %d)\t", i, adj->data);
        }
        printf("\n");
    }
}
/////////////////////////////////////////////////////
void free_graph(struct graph *G, struct list *edges)
{
    //Nodes -> node -> adj -> node -> adj
    //struct list *p_nodes = G->nodes;
    free_list(G->nodes);
    /*while(G->nodes != NULL)
      {
      struct list *tmp_nodes = G->nodes;


      struct node *n = G->nodes->elmt;
      free_list(n->adj);
    //free(n);

    free(p_nodes);
    p_nodes = tmp_nodes->next;
    }*/
    // free(p_nodes);
    // free(G);
    free_list(edges);
}

/////////////////////////////////////////////////
/*int main()
  {
//INIT A LIST OF EDGES
struct list *edges = list_init();

//CREATE FEW EDGES OBJECT
struct edge *e1 = new_edge(2, 3);
struct edge *e2 = new_edge(1, 0);
struct edge *e3 = new_edge(2, 4);

//UPDATE THE EDGES LIST
push_list_front(edges, (void *)e1);
struct edge *e = edges->next->elmt;
printf("%d\n", e->dest);
push_list_front(edges, (void *)e2);
push_list_front(edges, (void *)e3);

//CREATE A GRAPH AND PRINT IT
int nb_vertices = 8;
printf("Number of vertices : %d\n", nb_vertices);
printf("Number of edges : %d\n\n", len_list(edges));

struct graph *G = create_graph(0, nb_vertices, edges);
print_graph(G);

free_graph(G, edges);
return 1;
}*/
