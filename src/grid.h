#include "common.h"
#include "fabtrimino.h"

#define GRID_HEIGHT 40
#define GRID_WIDTH 10
#define GRID_VISIBLE_HEIGHT 20


struct grid;

struct grid* grid_make(SDL_Renderer*, SDL_Rect*);

void grid_receive(struct grid*, struct fabtrimino*);

void grid_draw(struct grid*);

void grid_rotate_piece_clockwise(struct grid*);

void grid_rotate_piece_counter_clockwise(struct grid*);
