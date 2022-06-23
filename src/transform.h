#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H
void save_graph(struct graph *G, const char *filepath);
struct graph *create_file(const char *filepath);
#endif
