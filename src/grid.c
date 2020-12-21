#include "grid.h"
#include "fabtrimino.h"

struct grid
{
    SDL_Renderer* renderer;
    SDL_Rect* viewport;
    struct fabtrimino* active_piece;
    struct square* matrix[GRID_WIDTH][GRID_HEIGHT];
    struct vector active_piece_pos;
    struct next* next_pieces;
};

const size_t GRID_OFFSET = 20;

struct grid* grid_make(SDL_Renderer* renderer, SDL_Rect* viewport, struct next* next)
{
    assert(renderer != NULL);
    assert(viewport != NULL);

    struct grid* grid = calloc(1, sizeof(*grid));
    memset(grid->matrix, 0, GRID_WIDTH*GRID_HEIGHT*sizeof(grid->matrix[0][0]));

    grid->renderer = renderer;
    grid->viewport = viewport;
    grid->active_piece = NULL;

    grid->next_pieces = next;

    return grid;
}

void grid_receive(struct grid* grid, struct fabtrimino* fab)
{
    if (grid->active_piece != NULL)
    {
        fab_free(grid->active_piece);
    }
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

    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        for (size_t j = 0 + GRID_OFFSET; j < GRID_HEIGHT; j++)
        {
            if (grid->matrix[i][j] != NULL)
            {
                struct vector position = {0};
                position.x = i;
                position.y = j - GRID_OFFSET;
                draw_square(grid->matrix[i][j], &position, grid->renderer);
            }
        }
    }
    struct vector visible_position = (struct vector) {
        .x = grid->active_piece_pos.x,
        .y = grid->active_piece_pos.y - GRID_VISIBLE_HEIGHT
    };
    fab_draw(grid->active_piece, grid->renderer, &visible_position);
}

void grid_rotate_piece_clockwise(struct grid* grid)
{
    fab_rotate_clockwise(grid->active_piece);
}

void grid_rotate_piece_counter_clockwise(struct grid* grid)
{
    fab_rotate_counter_clockwise(grid->active_piece);
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
    if (check_left(grid))
        {
            grid->active_piece_pos.x-=1;
        }
}

void grid_move_piece_right(struct grid* grid)
{
        if (check_right(grid))
        {
            grid->active_piece_pos.x+=1;
        }
}

void grid_piece_fall(struct grid* grid)
{
    if (check_under(grid))
    {
        grid->active_piece_pos.y+=1;
    }
    else
    {
        grid_lock_piece(grid);
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
        free_square(grid->matrix[i][line_number]);
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
    grid_receive(grid, next_pull(grid->next_pieces));
}
