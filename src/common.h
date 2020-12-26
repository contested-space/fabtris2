#pragma once
#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdint.h>

// These sweet macros have been stolen from Remi Attab
// https://github.com/RAttab
#define sdl_fail(p)                                                     \
    do {                                                                \
        fprintf(stderr, "sdl-fail<%s:%u> %s: %s\n",                     \
                __FILE__, __LINE__, #p, SDL_GetError());                \
        abort();                                                        \
    } while(false)


#define sdl_err(p)                                                      \
    ({                                                                  \
        typeof(p) ret = (p);                                            \
        if (unlikely(ret < 0)) sdl_fail(p);                             \
        ret;                                                            \
    })

#define sdl_ptr(p)                                                      \
    ({                                                                  \
        typeof(p) ret = (p);                                            \
        if (unlikely(!ret)) sdl_fail(p);                                \
        ret;                                                            \
    })

#define likely(x)    __builtin_expect(x, 1)
#define unlikely(x)  __builtin_expect(x, 0)

struct vector {
    int8_t x;
    int8_t y;
};

static const size_t SCREEN_WIDTH = 640;
static const size_t SCREEN_HEIGHT = 480;
static const size_t GRID_WIDTH = SCREEN_WIDTH/2;
static const size_t GRID_HEIGHT = SCREEN_HEIGHT;
static const size_t GRID_SQUARE_LENGTH = 24;

enum shape
{
    I, J, L, O, S, T, Z
};
