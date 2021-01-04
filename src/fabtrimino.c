#include "fabtrimino.h"
#include "matrix.h"
#include "square.h"

void fill_T_squares(struct square* matrix[4][4])
{
    matrix[0][1] = square_make(T);
    matrix[1][1] = square_make(T);
    matrix[2][1] = square_make(T);
    matrix[1][0] = square_make(T);
}

void fill_I_squares(struct square* matrix[4][4])
{
    matrix[0][1] = square_make(I);
    matrix[1][1] = square_make(I);
    matrix[2][1] = square_make(I);
    matrix[3][1] = square_make(I);
}

void fill_J_squares(struct square* matrix[4][4])
{
    matrix[0][0] = square_make(J);
    matrix[0][1] = square_make(J);
    matrix[1][1] = square_make(J);
    matrix[2][1] = square_make(J);
}

void fill_L_squares(struct square* matrix[4][4])
{
    matrix[0][1] = square_make(L);
    matrix[1][1] = square_make(L);
    matrix[2][1] = square_make(L);
    matrix[2][0] = square_make(L);
}

void fill_O_squares(struct square* matrix[4][4])
{
    matrix[1][1] = square_make(O);
    matrix[1][2] = square_make(O);
    matrix[2][1] = square_make(O);
    matrix[2][2] = square_make(O);
}

void fill_S_squares(struct square* matrix[4][4])
{
    matrix[1][0] = square_make(S);
    matrix[2][0] = square_make(S);
    matrix[0][1] = square_make(S);
    matrix[1][1] = square_make(S);
}

void fill_Z_squares(struct square* matrix[4][4])
{
    matrix[0][0] = square_make(Z);
    matrix[1][0] = square_make(Z);
    matrix[1][1] = square_make(Z);
    matrix[2][1] = square_make(Z);
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

    fab->rotation_state = 0;

    return fab;
}

void fab_free(struct fabtrimino* fab)
{
    for (size_t i = 0; i < 4; i++) // TODO: change the 4 to matrix.size once the matrix is a struct
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                square_free(fab->matrix[i][j]);
            }
        }
    }
    free(fab);
}

void fab_draw_moving(struct fabtrimino* fab, SDL_Renderer* renderer, struct vector* offset, int32_t partial_move)
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
                square_draw_moving(fab->matrix[i][j], &position, renderer, partial_move);
            }
        }
    }
}

void fab_draw(struct fabtrimino* fab, SDL_Renderer* renderer, struct vector* offset)
{
    fab_draw_moving(fab, renderer, offset, 0);
}

void fab_rotate_clockwise(struct fabtrimino* fab)
{
    size_t size = matrix_size(fab->shape);
    if (size != 0)
    {
        matrix_rotate_clockwise(fab->matrix, size);
        fab->rotation_state = (fab->rotation_state + 1) % 4;
    }

}

void fab_rotate_counter_clockwise(struct fabtrimino* fab)
{
    size_t size = matrix_size(fab->shape);
    if (size != 0)
    {
        matrix_rotate_counter_clockwise(fab->matrix, size);
        fab->rotation_state = (4 + fab->rotation_state - 1) % 4;
    }
}

void fab_copy_matrix(struct fabtrimino* fab, struct square* matrix[4][4])
{
    matrix_free(matrix);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                matrix[i][j] = square_make(fab->shape);
            }
        }
    }
}

void fab_set_matrix(struct fabtrimino* fab, struct square* matrix[4][4])
{
    matrix_free(fab->matrix);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (matrix[i][j] != NULL)
            {
                fab->matrix[i][j] = square_make(fab->shape);
            }
        }
    }
}
