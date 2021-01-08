#include "common.h"
#include "fabtrimino.h"

struct hold* hold_make(SDL_Renderer*, SDL_Rect*);

void hold_draw(struct hold*);

struct fabtrimino* hold_switch(struct hold*, struct fabtrimino*);
