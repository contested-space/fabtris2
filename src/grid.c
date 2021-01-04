#include "grid.h"
#include "fabtrimino.h"
#include "hold.h"
#include "square.h"
#include "matrix.h"

struct grid
{
    SDL_Renderer* renderer;
    SDL_Rect* viewport;
    struct fabtrimino* active_piece;
    struct square* matrix[GRID_WIDTH][GRID_HEIGHT];
    struct vector active_piece_pos;
    struct next* next_pieces;
    struct hold* held_piece;
    bool active_piece_is_moving_left;
    bool active_piece_is_moving_right;
    size_t movement_start_time;
    size_t fall_start_time;
};

const size_t GRID_OFFSET = 20;

struct grid* grid_make(SDL_Renderer* renderer, SDL_Rect* viewport, struct next* next, struct hold* hold)
{
    assert(renderer != NULL);
    assert(viewport != NULL);

    struct grid* grid = calloc(1, sizeof(*grid));
    memset(grid->matrix, 0, GRID_WIDTH*GRID_HEIGHT*sizeof(grid->matrix[0][0]));

    grid->renderer = renderer;
    grid->viewport = viewport;
    grid->active_piece = NULL;
    grid->next_pieces = next;
    grid->held_piece = hold;
    grid->active_piece_is_moving_left = false;
    grid->active_piece_is_moving_right = false;

    return grid;
}

void grid_update(struct grid* grid)
{
    if (grid->active_piece_is_moving_left || grid->active_piece_is_moving_right)
    {
        if (SDL_GetTicks() - grid->movement_start_time > MOVE_DURATION)
        {
            grid->active_piece_is_moving_left = false;
            grid->active_piece_is_moving_right = false;
        }
    }
    grid_piece_fall(grid);
}

void grid_receive(struct grid* grid, struct fabtrimino* fab)
{
    grid->fall_start_time = SDL_GetTicks();
    grid->active_piece = fab;
    grid->active_piece_pos = (struct vector) {.x = 3, .y = GRID_OFFSET - 1}; // TODO: adapt to shapes
}

void grid_draw(struct grid* grid)
{
    sdl_err(SDL_RenderSetViewport(grid->renderer, grid->viewport));
    sdl_err(SDL_SetRenderDrawColor(grid->renderer, 0x00, 0x00, 0x00, 0xFF));
    SDL_Rect background = {
        .x = 0,
        .y = 0,
        .w = grid->viewport->w,
        .h = grid->viewport->h
    };
    sdl_err(SDL_RenderFillRect(grid->renderer, &background));

    //TODO: Make visible grid optional
    sdl_err(SDL_SetRenderDrawColor(grid->renderer, 0x00, 0x13, 0xFF, 0xFF));
    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        sdl_err(SDL_RenderDrawLine(grid->renderer,
                                   i * GRID_SQUARE_LENGTH,
                                   0,
                                   i * GRID_SQUARE_LENGTH,
                                   GRID_HEIGHT * GRID_SQUARE_LENGTH));
    }
    for (size_t j = 0; j < GRID_HEIGHT; j++)
    {
        sdl_err(SDL_RenderDrawLine(grid->renderer,
                                   0,
                                   j * GRID_SQUARE_LENGTH,
                                   GRID_WIDTH * GRID_SQUARE_LENGTH,
                                   j * GRID_SQUARE_LENGTH));
    }

    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        for (size_t j = 0 + GRID_OFFSET; j < GRID_HEIGHT; j++)
        {
            if (grid->matrix[i][j] != NULL)
            {
                struct vector position = {0};
                position.x = i;
                position.y = j - GRID_OFFSET;
                square_draw(grid->matrix[i][j], &position, grid->renderer);
            }
        }
    }
    struct vector visible_position = (struct vector) {
        .x = grid->active_piece_pos.x,
        .y = grid->active_piece_pos.y - GRID_VISIBLE_HEIGHT
    };
    visible_position.y -= 1; //adjust drawing position to account for movement illusion
        int32_t partial_move_vertical = SDL_GetTicks() - grid->fall_start_time;
    if (grid->active_piece_is_moving_left)
    {
        int32_t partial_move_lateral = grid->movement_start_time - SDL_GetTicks();
        visible_position.x += 1; //also illusory adjustment
        fab_draw_moving(grid->active_piece,
                        grid->renderer,
                        &visible_position,
                        partial_move_lateral,
                        partial_move_vertical);
    }
    else if (grid->active_piece_is_moving_right)
    {
        int32_t partial_move_lateral = SDL_GetTicks() - grid->movement_start_time;
        visible_position.x -= 1; //here too
        fab_draw_moving(grid->active_piece,
                        grid->renderer,
                        &visible_position,
                        partial_move_lateral,
                        partial_move_vertical);
    }
    else
    {

        fab_draw_moving(grid->active_piece, grid->renderer, &visible_position, 0, partial_move_vertical);
    }
}

bool check_position(struct grid* grid, struct square* matrix[4][4], struct vector position)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrix[i][j] != NULL &&
                (position.x + i < 0 ||
                 position.x + i >= GRID_WIDTH ||
                 position.y + j >= GRID_HEIGHT))
            {
                return false;
            }
            if (matrix[i][j] != NULL && grid->matrix[i+position.x][j+position.y] != NULL)
            {
                return false;
            }
        }
    }
    return true;
}

//TODO: This is not OK. This needs to be refactored into
//      something that isn't 1000+ lines long
void grid_rotate_I_clockwise(struct grid* grid)
{
    struct square* matrix[4][4];
    memset(matrix, 0, 4*4*sizeof(struct square*));

    fab_copy_matrix(grid->active_piece, matrix);

    size_t size = matrix_size(grid->active_piece->shape);
    matrix_rotate_clockwise(matrix, size);

    struct vector position;

    bool can_rotate = false;

    switch (grid->active_piece->rotation_state) {
    case 0:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 1:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 2:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 3:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
    }
    //If all the tests have failed, the piece isn't rotated
    if (can_rotate)
    {
        fab_rotate_clockwise(grid->active_piece);
        grid->active_piece_pos.x = position.x;
        grid->active_piece_pos.y = position.y;
    }

    matrix_free(matrix);
}

void grid_rotate_I_counter_clockwise(struct grid* grid)
{
    struct square* matrix[4][4];
    memset(matrix, 0, 4*4*sizeof(struct square*));

    fab_copy_matrix(grid->active_piece, matrix);

    size_t size = matrix_size(grid->active_piece->shape);
    matrix_rotate_counter_clockwise(matrix, size);

    struct vector position;

    bool can_rotate = false;

    switch (grid->active_piece->rotation_state) {
    case 0:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 1:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x + 2;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 2:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 3:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x - 2;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
    }
    //If all the tests have failed, the piece isn't rotated
    if (can_rotate)
    {
        fab_rotate_counter_clockwise(grid->active_piece);
        grid->active_piece_pos.x = position.x;
        grid->active_piece_pos.y = position.y;
    }

    matrix_free(matrix);
}

void grid_rotate_default_clockwise(struct grid* grid)
{
    struct square* matrix[4][4];
    memset(matrix, 0, 4*4*sizeof(struct square*));

    fab_copy_matrix(grid->active_piece, matrix);

    size_t size = matrix_size(grid->active_piece->shape);
    matrix_rotate_clockwise(matrix, size);

    struct vector position;

    bool can_rotate = false;

    switch (grid->active_piece->rotation_state) {
    case 0:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 1:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 2:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 3:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
    }
    //If all the tests have failed, the piece isn't rotated
    if (can_rotate)
    {
        grid->active_piece_pos.x = position.x;
        grid->active_piece_pos.y = position.y;
        fab_rotate_clockwise(grid->active_piece);
    }

    matrix_free(matrix);
}

void grid_rotate_default_counter_clockwise(struct grid* grid)
{
    struct square* matrix[4][4];
    memset(matrix, 0, 4*4*sizeof(struct square*));

    fab_copy_matrix(grid->active_piece, matrix);

    size_t size = matrix_size(grid->active_piece->shape);
    matrix_rotate_counter_clockwise(matrix, size);

    struct vector position;

    bool can_rotate = false;

    switch (grid->active_piece->rotation_state) {
    case 0:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 1:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x + 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 2:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y - 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y + 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        break;
    case 3:
        //test 1
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 2
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 3
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y + 1;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 4
        position.x = grid->active_piece_pos.x;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
        // test 5
        position.x = grid->active_piece_pos.x - 1;
        position.y = grid->active_piece_pos.y - 2;
        if (check_position(grid, matrix, position))
        {
            can_rotate = true;
            break;
        }
    }
    //If all the tests have failed, the piece isn't rotated
    if (can_rotate)
    {
        fab_rotate_counter_clockwise(grid->active_piece);
        grid->active_piece_pos.x = position.x;
        grid->active_piece_pos.y = position.y;
    }

    matrix_free(matrix);
}


void grid_rotate_piece_clockwise(struct grid* grid)
{
    switch(grid->active_piece->shape) {
    case O:
        // O piece doesn't need to rotate
        break;
    case I:
        grid_rotate_I_clockwise(grid);
        break;
    case J:
    case L:
    case S:
    case Z:
    case T:
    default:
        grid_rotate_default_clockwise(grid);
    }
}

void grid_rotate_piece_counter_clockwise(struct grid* grid)
{
        switch(grid->active_piece->shape) {
    case O:
        // O piece doesn't need to rotate
        break;
    case I:
        grid_rotate_I_counter_clockwise(grid);
        break;
    case J:
    case L:
    case S:
    case Z:
    case T:
    default:
        grid_rotate_default_counter_clockwise(grid);
    }
}

bool check_left(struct grid* grid)
{
    struct vector offset = grid->active_piece_pos;
    struct fabtrimino* fab = grid->active_piece;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                if ((int8_t) i + offset.x - 1 < 0)
                {
                    return false;
                }
                if (grid->matrix[i + offset.x - 1][(int8_t) j + offset.y] != NULL)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool check_right(struct grid* grid)
{
    struct vector offset = grid->active_piece_pos;
    struct fabtrimino* fab = grid->active_piece;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                if ((int8_t) i + offset.x + 1 >= 10)
                {
                    return false;
                }
                if (grid->matrix[i + offset.x + 1][(int8_t) j + offset.y] != NULL)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool check_under(struct grid* grid)
{

    struct vector offset = grid->active_piece_pos;
    struct fabtrimino* fab = grid->active_piece;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (fab->matrix[i][j] != NULL)
            {
                if ((int8_t) j + offset.y + 1 >= GRID_HEIGHT)
                {
                    return false;
                }
                if (grid->matrix[i + offset.x][(int8_t) j + offset.y + 1] != NULL)
                {

                    return false;
                }
            }
        }
    }
    return true;
}

// TODO: add target based movement
void grid_move_piece_left(struct grid* grid)
{
    if (grid->active_piece_is_moving_left)
    {
        return;
    }
    if (grid->active_piece_is_moving_right)
    {
        //TODO: move left
        return;
    }
    if (check_left(grid))
        {
            grid->movement_start_time = SDL_GetTicks();
            grid->active_piece_is_moving_left = true;
            grid->active_piece_pos.x-=1;
        }
}

void grid_move_piece_right(struct grid* grid)
{

    if (grid->active_piece_is_moving_right)
    {
        return;
    }
    if (grid->active_piece_is_moving_left)
    {
        //TODO: move right
        return;
    }
        if (check_right(grid))
        {
            grid->movement_start_time = SDL_GetTicks();
            grid->active_piece_is_moving_right = true;
            grid->active_piece_pos.x+=1;
        }
}

void grid_piece_fall(struct grid* grid)
{
    // TODO: base duration on level
    uint32_t now = SDL_GetTicks();
    if (now > grid->fall_start_time + FALL_DURATION)
    {
        if (check_under(grid))
        {
            grid->active_piece_pos.y+=1;
            grid->fall_start_time = now;
        }
        else
        {
            grid_lock_piece(grid);
        }
    }
}

bool check_full_line(struct grid* grid, size_t line_number)
{
    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        if (grid->matrix[i][line_number] == NULL)
        {
            return false;
        }
    }
    return true;
}

void move_squares_down(struct grid* grid, size_t bottom)
{
    for (size_t j = bottom; j > 0; j--)
    {
        for (size_t i = 0; i < GRID_WIDTH; i++)
        {
            grid->matrix[i][j] = grid->matrix[i][j - 1];
        }
    }
    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        grid->matrix[i][0] = NULL;
    }

}

void clear_line(struct grid* grid, size_t line_number)
{
    for(size_t i = 0; i < GRID_WIDTH; i++)
    {
        square_free(grid->matrix[i][line_number]);
        grid->matrix[i][line_number] = NULL;
    }
}

void clear_lines(struct grid* grid)
{
    size_t cleared_lines[4];

    size_t nb_cleared_lines = 0;
    for (size_t i = 0; i < GRID_HEIGHT; i++)
    {
        if (check_full_line(grid, i))
        {
            nb_cleared_lines++;
            cleared_lines[nb_cleared_lines - 1] = i;
        }
    }

    // TODO: add a kickass effect highlighting all the cleared lines

    if (nb_cleared_lines > 0)
    {
        for (size_t i = 0; i < nb_cleared_lines; i++)
        {
            clear_line(grid, cleared_lines[i]);
            move_squares_down(grid, cleared_lines[i]);
        }
    }
}


void grid_lock_piece(struct grid* grid)
{
    struct vector offset = grid->active_piece_pos;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (grid->active_piece->matrix[i][j] != NULL)
            {
                struct square* square = calloc(1, sizeof(square));
                square->shape = grid->active_piece->matrix[i][j]->shape;
                grid->matrix[i+offset.x][j+offset.y] = square;
            }
        }
    }
    clear_lines(grid);
    fab_free(grid->active_piece);
    grid_receive(grid, next_pull(grid->next_pieces));
}

void grid_hold_piece(struct grid* grid)
{
    struct fabtrimino* new_held = grid->active_piece;
    struct fabtrimino* old_held = hold_switch(grid->held_piece, new_held);
    if (old_held == NULL)
    {
        grid_receive(grid, next_pull(grid->next_pieces));
    }
    else
    {
        grid_receive(grid, old_held);
    }
}
