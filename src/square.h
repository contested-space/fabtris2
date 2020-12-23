#pragma once
#include "common.h"

struct square
{
    enum shape shape;
};

struct square* square_make(enum shape shape);

void square_draw(struct square*, struct vector*, SDL_Renderer*);

void square_free(struct square*);
