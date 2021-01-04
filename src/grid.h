#include "common.h"
#include "fabtrimino.h"
#include "next.h"
#include "hold.h"

#define GRID_HEIGHT 40
#define GRID_WIDTH 10
#define GRID_VISIBLE_HEIGHT 20

struct grid* grid_make(SDL_Renderer*, SDL_Rect*, struct next*, struct hold*);

void grid_receive(struct grid*, struct fabtrimino*);

void grid_draw(struct grid*);

void grid_rotate_piece_clockwise(struct grid*);

void grid_rotate_piece_counter_clockwise(struct grid*);

void grid_move_piece_left(struct grid*);

void grid_move_piece_right(struct grid*);

void grid_piece_fall(struct grid*);

void grid_lock_piece(struct grid*);

void grid_hold_piece(struct grid*);

void grid_update(struct grid*);
