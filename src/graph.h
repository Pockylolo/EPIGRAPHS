#include "stdio.h"
#include "stdlib.h"

#ifndef GRAPH_H
#define GRAPH_H

///////////////////***STRUCT PART***/////////////////////
struct graph
{
    int oriented;        /*if the graph is oriented then oriented = 1*/
    int order;           /*nb of vertices*/
    struct list *nodes;  /*list which contains each nodes*/
};

struct node
{
    int data;            /*the id of the node*/
    struct list *adj;    /*adjacence list which contains nodes*/
};


struct edge
{
    int src;
    int dest;
};

////////////***FUNCTION PART***/////////////////////
struct graph *init_graph(int oriented, int order);
int graph_is_empty(struct graph *G);
struct node *find_node(struct graph *G, int i);
struct node* new_node(int data);
struct edge *new_edge(int src, int dest);
struct list *edges_from_graph(struct graph *G);
void print_graph(struct graph *G);
struct graph* create_graph(int oriented, int order, struct list *edges);
void free_graph(struct graph *G, struct list *edges);
int edges_equal(int oriented, struct edge *e1, struct edge *e2);
int is_in_list_edges(int oriented, struct list *l, struct edge *elm);
int* DFS_Prefix(struct graph *G);
int* DFS_Suffix(struct graph *G);

#endif
