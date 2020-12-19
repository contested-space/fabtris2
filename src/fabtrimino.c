#include "fabtrimino.h"

struct square
{
    enum shape shape;
};

struct square* make_square(enum shape shape)
{
    struct square* sqr = calloc(1, sizeof(*sqr));
    sqr->shape = shape;
    return sqr;
}

void fill_T_squares(struct square* matrix[4][4])
{
    matrix[0][1] = make_square(T);
    matrix[1][1] = make_square(T);
    matrix[2][1] = make_square(T);
    matrix[1][0] = make_square(T);
}

void fill_I_squares(struct square* matrix[4][4])
{
    matrix[0][1] = make_square(I);
    matrix[1][1] = make_square(I);
    matrix[2][1] = make_square(I);
    matrix[3][1] = make_square(I);
}

void fill_J_squares(struct square* matrix[4][4])
{
    matrix[0][0] = make_square(J);
    matrix[0][1] = make_square(J);
    matrix[1][1] = make_square(J);
    matrix[2][1] = make_square(J);
}

void fill_L_squares(struct square* matrix[4][4])
{
    matrix[0][1] = make_square(L);
    matrix[1][1] = make_square(L);
    matrix[2][1] = make_square(L);
    matrix[2][0] = make_square(L);
}

void fill_O_squares(struct square* matrix[4][4])
{
    matrix[1][1] = make_square(O);
    matrix[1][2] = make_square(O);
    matrix[2][1] = make_square(O);
    matrix[2][2] = make_square(O);
}

void fill_S_squares(struct square* matrix[4][4])
{
    matrix[1][0] = make_square(S);
    matrix[2][0] = make_square(S);
    matrix[0][1] = make_square(S);
    matrix[1][1] = make_square(S);
}

void fill_Z_squares(struct square* matrix[4][4])
{
    matrix[0][0] = make_square(Z);
    matrix[1][0] = make_square(Z);
    matrix[1][1] = make_square(Z);
    matrix[2][1] = make_square(Z);
}

void fill_shape_squares(struct square* matrix[4][4], enum shape shape)
{
    switch(shape){
    case T:
        fill_T_squares(matrix);
        break;
    case I:
        fill_I_squares(matrix);
        break;
    case J:
        fill_J_squares(matrix);
        break;
    case L:
        fill_L_squares(matrix);
        break;
    case Z:
        fill_Z_squares(matrix);
        break;
    case S:
        fill_S_squares(matrix);
        break;
    case O:
        fill_O_squares(matrix);
        break;
    default:
        printf("INVALID SHAPE: %d\n", shape);
        break;
    }
}

struct fabtrimino* fab_make(enum shape shape)
{
    struct fabtrimino* fab = calloc(1, sizeof(*fab));
    memset(fab->matrix, 0, 4*4*sizeof(fab->matrix[0][0]));
    fill_shape_squares(fab->matrix, shape);

    fab->shape = shape;

    fab->pos = (struct vector) {.x = 0, .y = 0};

    return fab;
}

void free_square(struct square* square)
{
    free(square);
}

void fab_free(struct fabtrimino* fab)
{
    for (size_t i = 0; i < 4; i++) // TODO: change the 4 to matrix.size once the matrix is a struct
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                free_square(fab->matrix[i][j]);
            }
        }
    }
    free(fab);
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

void draw_square(struct square* square, struct vector* position, SDL_Renderer* renderer)
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

void fab_draw(struct fabtrimino* fab, SDL_Renderer* renderer, struct vector* offset)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                struct vector position = {0};
                position.x = i + offset->x;
                position.y = j + offset->y;
                draw_square(fab->matrix[i][j], &position, renderer);
            }
        }
    }
}


// TODO: clean rotation using a single temp square
void rotate_clockwise(struct square* matrix[4][4], size_t size)
{
    struct square* temp[4][4];
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            temp[i][j] = matrix[i][j];
        }
    }
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matrix[i][j] = temp[j][size-i-1];
        }
    }
}

// TODO: not that
void rotate_counter_clockwise(struct square* matrix[4][4], size_t size)
{
  rotate_clockwise(matrix, size);
  rotate_clockwise(matrix, size);
  rotate_clockwise(matrix, size);
}

size_t size_of_matrix(enum shape shape)
{
    switch(shape) {
    case T:
    case S:
    case Z:
    case L:
    case J:
        return 3;
    case I:
        return 4;
    case O:
    default:
        return 0;
    }
}

void fab_rotate_clockwise(struct fabtrimino* fab)
{
    size_t size = size_of_matrix(fab->shape);
    if (size != 0)
    {
        rotate_clockwise(fab->matrix, size);
    }
}

void fab_rotate_counter_clockwise(struct fabtrimino* fab)
{
    size_t size = size_of_matrix(fab->shape);
    if (size != 0)
    {
        rotate_counter_clockwise(fab->matrix, size);
    }
}
