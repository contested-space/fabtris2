#include "matrix.h"
#include "square.h"

void matrix_rotate_clockwise(struct square* matrix[4][4], size_t size)
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

void matrix_rotate_counter_clockwise(struct square* matrix[4][4], size_t size)
{
  matrix_rotate_clockwise(matrix, size);
  matrix_rotate_clockwise(matrix, size);
  matrix_rotate_clockwise(matrix, size);
}

void matrix_free(struct square* matrix[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrix[i][j] != NULL)
            {
                square_free(matrix[i][j]);
            }
        }
    }
}

size_t matrix_size(enum shape shape)
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
