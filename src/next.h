#pragma once
#include<SDL2/SDL.h>
#include "common.h"

struct next;

struct next* next_make(SDL_Renderer*, SDL_Rect*);

void next_draw(struct next*);

struct fabtrimino* next_pull(struct next*);
