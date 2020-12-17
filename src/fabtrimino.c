#include "fabtrimino.h"

struct square
{
    enum shape shape;
};

struct fabtrimino
{
    enum shape shape;
    struct square* matrix[4][4];
    struct vector* pos;
};


void nullify_squares_matrix(struct square* mat[4][4])
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            mat[i][j] = NULL;
        }
    }
}

struct square* make_square(enum shape shape)
{
    struct square* sqr = calloc(1, sizeof(*sqr));
    sqr->shape = shape;
    return sqr;
}

void fill_T_squares(struct square* matrix[4][4])
{
    matrix[0][2] = make_square(T);
    matrix[1][2] = make_square(T);
    matrix[2][2] = make_square(T);
    matrix[1][1] = make_square(T);
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
    matrix[0][1] = make_square(J);
    matrix[0][2] = make_square(J);
    matrix[1][2] = make_square(J);
    matrix[2][2] = make_square(J);
}

void fill_L_squares(struct square* matrix[4][4])
{
    matrix[0][2] = make_square(L);
    matrix[1][2] = make_square(L);
    matrix[2][2] = make_square(L);
    matrix[2][1] = make_square(L);
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
    matrix[1][1] = make_square(S);
    matrix[2][1] = make_square(S);
    matrix[0][2] = make_square(S);
    matrix[1][2] = make_square(S);
}

void fill_Z_squares(struct square* matrix[4][4])
{
    matrix[0][1] = make_square(Z);
    matrix[1][1] = make_square(Z);
    matrix[1][2] = make_square(Z);
    matrix[2][2] = make_square(Z);
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

    nullify_squares_matrix(fab->matrix);

    fill_shape_squares(fab->matrix, shape);

    fab->shape = shape;
    fab->pos = NULL;

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
    if (fab->pos != NULL)
    {
        free(fab->pos);
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
        col.r = 0x80;
        col.g = 0x00;
        col.b = 0x80;
        return col;
    case I:
        col.r = 0x00;
        col.g = 0xFF;
        col.b = 0xFF;
        return col;
    case L:
        col.r = 0xFF;
        col.g = 0xA5;
        col.b = 0x00;
        return col;
    case J:
        col.r = 0x00;
        col.g = 0x00;
        col.b = 0xFF;
        return col;
    case S:
        col.r = 0x00;
        col.g = 0x80;
        col.b = 0x00;
        return col;
    case Z:
        col.r = 0xFF;
        col.g = 0x00;
        col.b = 0x00;
        return col;
    case O:
        col.r = 0xFF;
        col.g = 0xFF;
        col.b = 0x00;
        return col;
    default:
        col.r = 0xAA;
        col.g = 0xBB;
        col.b = 0xCC;
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
                struct vector* position = calloc(1, sizeof(*position));
                position->x = i + offset->x;
                position->y = j + offset->y;
                draw_square(fab->matrix[i][j], position, renderer);
            }
        }
    }
}
