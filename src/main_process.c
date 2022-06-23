#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graph.h"
#include "list.h"
#include <string.h>
#include "graph_display.h"
#include "pixel.h"

#define size_img 106
#define size_back 700

#define pos_0x 5
#define pos_0y 150

#define pos_1x 140
#define pos_1y 5

#define pos_2x 430
#define pos_2y 5

#define pos_3x 590
#define pos_3y 150

#define pos_4x 590
#define pos_4y 410

#define pos_5x 430
#define pos_5y 590

#define pos_6x 140
#define pos_6y 590

#define pos_7x 5
#define pos_7y 410

////---SOLVED GRID FILE INTO PNG IMAGE GRID---////
void ImgReconst(SDL_Surface* Img, SDL_Surface* Back, int k)
{
    Uint32 pixel;
    Uint32 white = 0xFFFFFFFF;

    for(int i=0;i<size_img;i++)
        for(int j=0;j<size_img;j++)
        {
            pixel = get_pixel(Img,i,j);
            if(pixel == white)
                continue;
            if (k == 0)
                put_pixel(Back, i + pos_0x, j + pos_0y, pixel);
            else if (k == 1)
                put_pixel(Back, i + pos_1x, j + pos_1y, pixel);
            else if (k == 2)
                put_pixel(Back,i + pos_2x, j + pos_2y, pixel);
            else if (k == 3)
                put_pixel(Back, i + pos_3x, j + pos_3y, pixel);
            else if (k == 4)
                put_pixel(Back, i + pos_4x, j + pos_4y, pixel);
            else if (k == 5)
                put_pixel(Back, i + pos_5x, j + pos_5y, pixel);
            else if (k == 6)
                put_pixel(Back, i + pos_6x, j + pos_6y, pixel);
            else if (k == 7)
                put_pixel(Back, i + pos_7x, j + pos_7y, pixel);
        }
    SDL_UnlockSurface(Img);
    SDL_FreeSurface(Img);

   }

void display_base_graph(struct graph *G, struct list *edges)
{
    //Convert grid image into bmp format (surface)
    SDL_Surface* BackImg = Convert("images/background.png");
    Uint32 pixel;
    Uint32 white = 0xFFFFFFFF;

    /*PROCESS THE EDGES IMAGES 
     * IF IT THE EDGES IMAGE IS A WHITE PIXEL THEN NOTHING*/
    while(edges->next != NULL)
    {
        edges = edges->next;
        struct edge *e = edges->elmt;

        SDL_Surface* EdgeImg;
        if(G->oriented != 1) //non-oriented case
        {
            char path_edges[13] = "edges/__.png\0";

            if(e->src <= e->dest)
            {
                path_edges[6] = e->src + '0';
                path_edges[7] = e->dest + '0';
            }
            else
            {
                path_edges[6] = e->dest + '0';
                path_edges[7] = e->src + '0';
            }
            EdgeImg = Convert(path_edges);
        }
        else
        {
            char path_edges[22] = "edges_oriented/__.png\0";

            path_edges[15] = e->src + '0';
            path_edges[16] = e->dest + '0';
            EdgeImg = Convert(path_edges);
        }

        for (int i = 0; i<size_back; i++)
        {
            for (int j = 0; j<size_back; j++)
            {
                pixel = get_pixel(EdgeImg, i, j);
                if (pixel == white)
                    continue;
                put_pixel(BackImg, i, j, pixel);
            }
        }
        SDL_UnlockSurface(EdgeImg);
        SDL_FreeSurface(EdgeImg);
    }
    /*PROCESS THE VERTICES IMAGES*/
    char path[15] = "vertices/_.png\0";

    for(int k=0;k<G->order;k++)
    {
        path[9] = k + '0';
        SDL_Surface* NodeImg = Convert(path);
        ImgReconst(NodeImg, BackImg, k);
    }
     SDL_UnlockSurface(BackImg);
     IMG_SavePNG(BackImg,"images/graph_nodes.png");
     SDL_FreeSurface(BackImg);
}

void ColorNode(int node, char* color)
{
    /*SDL_Surface* Cpy= Convert("images/graph_nodes.png");
    SDL_UnlockSurface(Cpy);
    IMG_SavePNG(Cpy, "images/colored_graph.png");*/

    SDL_Surface* ColorNought = Convert("images/green.png");
    SDL_Surface* BackImg = Convert("images/colored_graph.png");

    if (strcmp(color, "red") == 0)
        ColorNought = Convert("images/red.png");

    ImgReconst(ColorNought, BackImg, node);

    SDL_UnlockSurface(BackImg);
    IMG_SavePNG(BackImg,"images/colored_graph.png");
    SDL_FreeSurface(BackImg);
}

///***SAVE A GRAPH INTO A FILE***///
/*void save(struct graph *G, char *file_name)
  {
////SAVE THE GRID////
FILE *file_out;
file_out=fopen(file_name,"w");

fputc("la data",file_out);
putc(' ',file_out);

fclose(file_out);
}*/


/////***MAIN PROGRAM***///

struct graph* main_process_graph(int oriented, int order, struct list *edges)
{
    struct graph *G = create_graph(oriented, order, edges);
    display_base_graph(G, edges);
    print_graph(G);
    return G;
}


