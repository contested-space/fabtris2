#include "common.h"
#include "square.h"

void matrix_rotate_clockwise(struct square* matrix[4][4], size_t size);

void matrix_rotate_counter_clockwise(struct square* matrix[4][4], size_t size);

void matrix_free(struct square* matrix[4][4]);

size_t matrix_size(enum shape);
