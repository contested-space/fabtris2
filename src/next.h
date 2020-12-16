#pragma once
#include<SDL2/SDL.h>
#include "common.h"

struct next;

//fabtrimino* next_pop();

//void next_push(square*);

struct next* next_make(SDL_Renderer*, SDL_Rect*);

void next_draw(struct next*);

struct fabtrimino* next_pull(struct next*);
