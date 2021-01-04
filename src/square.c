#include "square.h"
#include <math.h>

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

int32_t lerp(int32_t origin_start, int32_t origin_end, int32_t origin_position, int32_t destination_start, int32_t destination_end)
{
    float ratio = (float) (origin_position - origin_start) / (origin_end - origin_start);
    int32_t adjustment = (int32_t) round(ratio * (destination_end - destination_start));
    return destination_start + adjustment;
}

void square_draw_moving(struct square* square, struct vector* position, SDL_Renderer* renderer, int32_t partial_move)
{
    struct color color = shape_color(square->shape);

    sdl_err(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF));

    int32_t moving_adjustment = lerp(0, MOVE_DURATION, partial_move, 0, GRID_SQUARE_LENGTH);
    SDL_Rect s = {
        .x = position->x * GRID_SQUARE_LENGTH + moving_adjustment,
        .y = position->y * GRID_SQUARE_LENGTH,
        .w = GRID_SQUARE_LENGTH,
        .h = GRID_SQUARE_LENGTH
    };
    sdl_err(SDL_RenderFillRect(renderer, &s));
}

void square_draw(struct square* square, struct vector* position, SDL_Renderer* renderer)
{
    square_draw_moving(square, position, renderer, 0);
}
