#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stddef.h>

#include "common.h"
#include "fabtrimino.h"
#include "next.h"
#include "grid.h"
#include "hold.h"

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
    SDL_Rect next_viewport = {
        .x = SCREEN_WIDTH - viewport_width,
        .y = 0,
        .w = viewport_width,
        .h = GRID_SQUARE_LENGTH * 20 //allows for 5 pieces
    };

    struct next* next_screen = next_make(renderer, &next_viewport);

    SDL_Rect hold_viewport = {
        .x = 0,
        .y = 0,
        .w = GRID_SQUARE_LENGTH * 4,
        .h = GRID_SQUARE_LENGTH * 4 //allows for 5 pieces
    };
    struct hold* hold_screen = hold_make(renderer, &hold_viewport);

    SDL_Rect grid_viewport = {
        .x = SCREEN_WIDTH/4, // This is sketchy. Must be fixed at some point
        .y = 0,
        .w = GRID_WIDTH * GRID_SQUARE_LENGTH,
        .h = GRID_VISIBLE_HEIGHT * GRID_SQUARE_LENGTH
    };

    struct grid* grid_screen = grid_make(renderer, &grid_viewport, next_screen, hold_screen);

    struct fabtrimino* first_piece = next_pull(next_screen);
    grid_receive(grid_screen, first_piece);


    SDL_Rect full_viewport = {
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
        }
        const uint8_t* current_key_states = SDL_GetKeyboardState(NULL);
        if (current_key_states[SDL_SCANCODE_X])
        {
            grid_rotate_piece_clockwise(grid_screen);
        }
        if (current_key_states[SDL_SCANCODE_Z])
        {
            grid_rotate_piece_counter_clockwise(grid_screen);
        }
        if (current_key_states[SDL_SCANCODE_LEFT])
        {
            grid_move_piece_left(grid_screen);
        }
        if (current_key_states[SDL_SCANCODE_RIGHT])
        {
            grid_move_piece_right(grid_screen);
        }
        if (current_key_states[SDL_SCANCODE_DOWN])
        {
            grid_piece_fall(grid_screen);
        }
        if (current_key_states[SDL_SCANCODE_SPACE])
        {
            grid_hold_piece(grid_screen);
        }
        grid_update(grid_screen);
        sdl_err(SDL_RenderSetViewport(renderer, &full_viewport));
        sdl_err(SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));
        sdl_err(SDL_RenderClear(renderer));
        next_draw(next_screen);
        grid_draw(grid_screen);
        hold_draw(hold_screen);
        SDL_RenderPresent(renderer);
    }
}
