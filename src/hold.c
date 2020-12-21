#include "hold.h"

struct hold
{
    SDL_Renderer* renderer;
    SDL_Rect* viewport;
    struct fabtrimino* held_piece;
};

struct hold* hold_make(SDL_Renderer* renderer, SDL_Rect* viewport)
{
    assert(renderer != NULL);
    assert(viewport != NULL);

    struct hold* hold = calloc(1, sizeof(*hold));

    hold->renderer = renderer;
    hold->viewport = viewport;
    hold->held_piece = NULL;

    return hold;
}


void hold_draw(struct hold* hold)
{
    sdl_err(SDL_RenderSetViewport(hold->renderer, hold->viewport));
    sdl_err(SDL_SetRenderDrawColor(hold->renderer, 0x00, 0x00, 0x00, 0xFF));
    SDL_Rect background = {
        .x = 0,
        .y = 0,
        .w = hold->viewport->w,
        .h = hold->viewport->h
    };
    sdl_err(SDL_RenderFillRect(hold->renderer, &background));
    struct vector held_position = (struct vector) {.x = 0, .y = 0};
    if (hold->held_piece != NULL)
    {
        fab_draw(hold->held_piece, hold->renderer, &held_position);
    }
}

struct fabtrimino* hold_switch(struct hold* hold, struct fabtrimino* fab)
{
    struct fabtrimino* ret = hold->held_piece;
    hold->held_piece = fab;
    return ret;
}
