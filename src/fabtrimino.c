#include "fabtrimino.h"

struct square
{
    enum shape shape;
};

struct fabtrimino
{
    enum shape shape;
    struct square*** matrix;
    struct vector* pos;
};


void nullify_squares_matrix(struct square*** mat, size_t size)
// TODO: catch and print error when size > matrix_size
// (maybe create struct for matrix with size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
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

void fill_T_squares(struct square*** matrix)
{
    matrix[0][1] = make_square(T);
    matrix[1][1] = make_square(T);
    matrix[2][1] = make_square(T);
    matrix[1][0] = make_square(T);
}


void fill_I_squares(struct square*** matrix)
{
    matrix[0][1] = make_square(T);
    matrix[1][1] = make_square(T);
    matrix[2][1] = make_square(T);
    matrix[3][1] = make_square(T);
}


void fill_J_squares(struct square*** matrix)
{
    matrix[0][0] = make_square(T);
    matrix[0][1] = make_square(T);
    matrix[1][1] = make_square(T);
    matrix[2][1] = make_square(T);
}


void fill_L_squares(struct square*** matrix)
{
    matrix[0][1] = make_square(T);
    matrix[1][1] = make_square(T);
    matrix[2][1] = make_square(T);
    matrix[2][0] = make_square(T);
}


void fill_O_squares(struct square*** matrix)
{
    matrix[1][1] = make_square(T);
    matrix[1][2] = make_square(T);
    matrix[2][1] = make_square(T);
    matrix[2][2] = make_square(T);
}


void fill_S_squares(struct square*** matrix)
{
    matrix[1][0] = make_square(T);
    matrix[2][0] = make_square(T);
    matrix[0][1] = make_square(T);
    matrix[1][1] = make_square(T);
}


void fill_Z_squares(struct square*** matrix)
{
    matrix[0][0] = make_square(T);
    matrix[1][0] = make_square(T);
    matrix[1][1] = make_square(T);
    matrix[2][1] = make_square(T);
}

void fill_shape_squares(struct square*** matrix, enum shape shape)
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

    fab->matrix = calloc(4, sizeof(struct square**));
    for (int i = 0; i<4; i++)
    {
        fab->matrix[i] = calloc(4, sizeof(struct square*));
    }
    nullify_squares_matrix(fab->matrix, 4);

    fill_shape_squares(fab->matrix, shape);

    fab->shape = shape;
    fab->pos = NULL;

    return fab;
}

void draw_square(struct square* square, struct vector* position, SDL_Renderer* renderer)
{

    //TODO replace with a color based on square->shape
    SDL_SetRenderDrawColor(renderer, 0x22, 0x33, 0x44, 0xFF);

    SDL_Rect s = {
        .x = position->x * GRID_SQUARE_LENGTH,
        .y = position->y * GRID_SQUARE_LENGTH,
        .w = GRID_SQUARE_LENGTH,
        .h = GRID_SQUARE_LENGTH
    };
    SDL_RenderFillRect(renderer, &s);
}

void fab_draw(struct fabtrimino* fab, SDL_Renderer* renderer)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                struct vector* position = calloc(1, sizeof(*position));
                position->x = i;
                position->y = j;
                draw_square(fab->matrix[i][j], position, renderer);
            }
        }
    }

}

void fab_free_square(struct square* square)
{
    free(square);
}
