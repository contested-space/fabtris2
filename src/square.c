#include "square.h"

struct square* square_make(enum shape shape)
{
    struct square* sqr = calloc(1, sizeof(*sqr));
    sqr->shape = shape;
    return sqr;
}

void square_free(struct square* square)
{
    free(square);
}

struct color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

struct color shape_color(enum shape shape)
{
    struct color col;
    switch(shape) {
    case T:
        col = (struct color) { .r = 0x80, .g = 0x00, .b = 0x80 };
        return col;
    case I:
        col = (struct color) { .r = 0x00, .g = 0xFF, .b = 0xFF };
        return col;
    case L:
        col = (struct color) { .r = 0xFF, .g = 0xA5, .b = 0x00 };
        return col;
    case J:
        col = (struct color) { .r = 0x00, .g = 0x00, .b = 0xFF };
        return col;
    case S:
        col = (struct color) { .r = 0x00, .g = 0x80, .b = 0x00 };
        return col;
    case Z:
        col = (struct color) { .r = 0xFF, .g = 0x00, .b = 0x00 };
        return col;
    case O:
        col = (struct color) { .r = 0xFF, .g = 0xFF, .b = 0x00 };
        return col;
    default:
        col = (struct color) { .r = 0xFF, .g = 0xAA, .b = 0xBB };
        return col;
    }
}

void square_draw(struct square* square, struct vector* position, SDL_Renderer* renderer)
{
    struct color color = shape_color(square->shape);

    sdl_err(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF));

    SDL_Rect s = {
        .x = position->x * GRID_SQUARE_LENGTH,
        .y = position->y * GRID_SQUARE_LENGTH,
        .w = GRID_SQUARE_LENGTH,
        .h = GRID_SQUARE_LENGTH
    };
    sdl_err(SDL_RenderFillRect(renderer, &s));
}
