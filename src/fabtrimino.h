#pragma once
#include<stddef.h>
#include<SDL2/SDL.h>
#include<common.h>
struct grid; //TODO: move to its own thing
//struct fabtrimino;
struct square;


enum shape
{
    I, J, L, O, S, T, Z
};

struct fabtrimino* fab_make(enum shape);

/* void fab_free(struct fabtrimino); */

/* void fab_draw(struct fabtrimino, SDL_Renderer*, SDL_Rect, vector pos); */

void fab_draw(struct fabtrimino*, SDL_Renderer*, struct vector*);
