#include "grid.h"

struct grid
{
    SDL_Renderer* renderer;
    SDL_Rect* viewport;
    struct fabtrimino* active_piece;
    struct square* matrix[GRID_WIDTH][GRID_HEIGHT];
};

struct grid* grid_make(SDL_Renderer* renderer, SDL_Rect* viewport)
{
    assert(renderer != NULL);
    assert(viewport != NULL);

    struct grid* grid = calloc(1, sizeof(*grid));
    memset(grid->matrix, 0, GRID_WIDTH*GRID_HEIGHT*sizeof(grid->matrix[0][0]));

    grid->renderer = renderer;
    grid->viewport = viewport;
    grid->active_piece = NULL;

    return grid;
}

void grid_draw(struct grid* g)
{
    sdl_err(SDL_RenderSetViewport(g->renderer, g->viewport));
    sdl_err(SDL_SetRenderDrawColor(g->renderer, 0x00, 0x00, 0x00, 0xFF));
    SDL_Rect background = {
        .x = 0,
        .y = 0,
        .w = g->viewport->w,
        .h = g->viewport->h
    };
    sdl_err(SDL_RenderFillRect(g->renderer, &background));
}
