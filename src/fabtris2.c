#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stddef.h>

#include "common.h"
#include "fabtrimino.h"
#include "next.h"
#include "grid.h"

SDL_Window* createWindow(const char* title)
{
    SDL_Window * window = sdl_ptr(SDL_CreateWindow(title,
                                                   SDL_WINDOWPOS_UNDEFINED,
                                                   SDL_WINDOWPOS_UNDEFINED,
                                                   SCREEN_WIDTH,
                                                   SCREEN_HEIGHT,
                                                   SDL_WINDOW_SHOWN));
    assert(window != NULL);
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = sdl_ptr(SDL_CreateRenderer(window,
                                                        -1,
                                                        SDL_RENDERER_ACCELERATED));
    assert(renderer != NULL);
    return renderer;
}

int main(int argc, char* argv[])
{

    (void) argc, (void) argv;

    SDL_Window* gameWindow = sdl_ptr(createWindow("Fabtris 2"));
    SDL_Renderer* renderer = sdl_ptr(createRenderer(gameWindow));

    SDL_Event e;


    size_t viewport_width = GRID_SQUARE_LENGTH * 6; //leaves 1 square on each size
    SDL_Rect nextViewport = {
        .x = SCREEN_WIDTH - viewport_width,
        .y = 0,
        .w = viewport_width,
        .h = GRID_SQUARE_LENGTH * 20 //allows for 5 pieces
    };

    struct next* next_screen = next_make(renderer, &nextViewport);



    SDL_Rect gridViewport = {
        .x = SCREEN_WIDTH/4, // This is sketchy. Must be fixed at some point
        .y = 0,
        .w = GRID_WIDTH * GRID_SQUARE_LENGTH,
        .h = GRID_VISIBLE_HEIGHT * GRID_SQUARE_LENGTH
    };

    struct grid* grid_screen = grid_make(renderer, &gridViewport);


    SDL_Rect fullViewport = {
        .x = 0,
        .y = 0,
        .w = SCREEN_WIDTH,
        .h = SCREEN_HEIGHT
    };

    while (true)
    {
        while(sdl_err(SDL_PollEvent(&e)) != 0)
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
        sdl_err(SDL_RenderSetViewport(renderer, &fullViewport));
        sdl_err(SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));
        sdl_err(SDL_RenderClear(renderer));

        next_draw(next_screen);
        grid_draw(grid_screen);

        SDL_RenderPresent(renderer);
    }
}
