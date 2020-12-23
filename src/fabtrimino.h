#pragma once
#include "common.h"

struct fabtrimino
{
    enum shape shape;
    struct square* matrix[4][4];
    struct vector pos;
};

struct fabtrimino* fab_make(enum shape);

void fab_free(struct fabtrimino*);

void fab_draw(struct fabtrimino*, SDL_Renderer*, struct vector*);

void fab_rotate_clockwise(struct fabtrimino*);

void fab_rotate_counter_clockwise(struct fabtrimino*);
