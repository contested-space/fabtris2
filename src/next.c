#include "next.h"
#include "fabtrimino.h"

const size_t NB_NEXT = 5;

struct next {
    SDL_Renderer* renderer;
    SDL_Rect* viewport;
    struct fabtrimino** next_pieces;
    size_t index;
};

struct next* next_make(SDL_Renderer* renderer, SDL_Rect* viewport){
    assert(renderer != NULL);
    struct next *n = calloc(1, sizeof(*n));
    n->renderer = renderer;
    n->viewport = viewport;
    n->next_pieces = calloc(NB_NEXT, sizeof(n->next_pieces));;
    for (size_t i = 0; i < NB_NEXT; i++)
    {
        n->next_pieces[i] = fab_make(i); //TODO: replace by a randomizer
    }
    return n;
}

void next_draw(struct next* n)
{
    sdl_err(SDL_RenderSetViewport(n->renderer, n->viewport));
    sdl_err(SDL_SetRenderDrawColor(n->renderer, 0x00, 0x00, 0x00, 0xFF));
    SDL_Rect background = {
        .x = 0,
        .y = 0,
        .w = n->viewport->w,
        .h = n->viewport->h
    };
    sdl_err(SDL_RenderFillRect(n->renderer, &background));

    for (size_t i = 0; i < NB_NEXT; i++)
    {
        size_t i_prime = (i + n->index) % NB_NEXT;
        struct vector* offset = calloc(1, sizeof(*offset));
        offset->x = 1;
        offset->y = i * 4;
        fab_draw(n->next_pieces[i_prime], n->renderer, offset);
        free(offset);
    }
}

void next_insert(struct next* n, struct fabtrimino* fab)
{
    n->next_pieces[n->index] = fab;
    n->index = (n->index+1) % NB_NEXT;
}

struct fabtrimino* next_pull(struct next* n)
{
    struct fabtrimino* next = n->next_pieces[n->index];
    struct fabtrimino* new = fab_make(rand() % 7); // TODO: use a randomizer
    next_insert(n, new);
    return next;
}
