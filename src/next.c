#include<next.h>
#include<fabtrimino.h>

const size_t NB_NEXT = 3;

struct next {
    SDL_Renderer* renderer;
    SDL_Rect* viewport;
    struct fabtrimino* piece;
};

struct next* next_make(SDL_Renderer* renderer, SDL_Rect* viewport){
    assert(renderer != NULL);
    struct next *n = calloc(1, sizeof(*n));
    struct vector pos = {.x = 2, .y = 2};
    n->renderer = renderer;
    n->viewport = viewport;
    n->piece = fab_make(Z); //TODO: remove that thing
    return n;
}

void next_draw(struct next* n)
{
    assert(n->piece != NULL);
    SDL_RenderSetViewport(n->renderer, n->viewport);
    SDL_SetRenderDrawColor(n->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_Rect background = {
        .x = 0,
        .y = 0,
        .w = n->viewport->w,
        .h = n->viewport->h
    };
    SDL_RenderFillRect(n->renderer, &background);
    fab_draw(n->piece, n->renderer);

}
