#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include "graph_display.h"
#include <err.h>


///***VERTICE***///
//32 bits image format, resolution 106*106
#define SIZE_IMG  106

///***BACKGROUND***///
//32 bits image format, resolution 1000*1000
#define MAIN_SIZE 700


////--CONVERT png into bmp (an image into a surface)--////
SDL_Surface* Convert(char* path)
{
    SDL_Surface* surface;
    surface = IMG_Load(path);
    if(!surface)
        errx(1,"The file doesn't exist");

    SDL_Surface *tmp;
    tmp = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);

    SDL_FreeSurface(surface);

    surface = tmp;

    SDL_LockSurface(surface);

    return surface;
}



