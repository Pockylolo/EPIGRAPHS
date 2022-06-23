#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef MAIN_PROCESS_H
#define MAIN_PROCESS_H

struct graph *main_process_graph(int oriented, int order, struct list *edges);
void ImgReconst(SDL_Surface *Img, SDL_Surface *Back, int k);
void ColorNode(int node, char *color);
void display_base_graph(struct graph *G, struct list *edges);


#endif
