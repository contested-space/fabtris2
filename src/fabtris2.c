#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stddef.h>

#include "common.h"
#include "fabtrimino.h"
#include "next.h"

SDL_Window* createWindow(const char* title)
{
    SDL_Window * window = SDL_CreateWindow(title,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SCREEN_WIDTH,
                                           SCREEN_HEIGHT,
                                           SDL_WINDOW_SHOWN);
    assert(window != NULL);
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);
    assert(renderer != NULL);
    return renderer;
}

int main(int argc, char* args[])
{
    SDL_Window* gameWindow = createWindow("Fabtris 2");
    SDL_Renderer* renderer = createRenderer(gameWindow);

    SDL_Event e;

    SDL_Rect* nextViewport = calloc(1, sizeof(*nextViewport));
    int width = GRID_SQUARE_LENGTH * 6; //leaves 1 square on each size
    nextViewport->x = SCREEN_WIDTH - width;
    nextViewport->y = 0;
    nextViewport->w = width;
    nextViewport->h = GRID_SQUARE_LENGTH * 20; //allows for 5 pieces

    struct next* next_screen = next_make(renderer, nextViewport);

    SDL_Rect fullViewport = {
        .x = 0,
        .y = 0,
        .w = SCREEN_WIDTH,
        .h = SCREEN_HEIGHT
    };

    while (1)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                return 0;
            }
            if (e.type == SDL_KEYDOWN)
            {
                struct fabtrimino* fab = next_pull(next_screen);
                fab_free(fab);
            }
        }
        SDL_RenderSetViewport(renderer, &fullViewport);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        next_draw(next_screen);

        SDL_RenderPresent(renderer);
    }
}
