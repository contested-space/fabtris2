#pragma once
#include "common.h"

struct fabtrimino
{
    enum shape shape;
    struct square* matrix[4][4];
    struct vector pos;
    uint8_t rotation_state;
};

struct fabtrimino* fab_make(enum shape);

void fab_free(struct fabtrimino*);

void fab_draw(struct fabtrimino*, SDL_Renderer*, struct vector*);

void fab_draw_moving(struct fabtrimino*, SDL_Renderer*, struct vector*, int32_t);

void fab_rotate_clockwise(struct fabtrimino*);

void fab_rotate_counter_clockwise(struct fabtrimino*);

void fab_copy_matrix(struct fabtrimino*, struct square* matrix[4][4]);
