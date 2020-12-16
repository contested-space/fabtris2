#pragma once
#include<assert.h>

struct vector {
    int x;
    int y;
};

/* extern const int SCREEN_WIDTH; */
/* extern const int SCREEN_HEIGHT; */
/* extern const int GRID_WIDTH; */
/* extern const int GRID_HEIGHT; */
/* extern const int GRID_SQUARE_LENGTH; */

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;
static const int GRID_WIDTH = SCREEN_WIDTH/2;
static const int GRID_HEIGHT = SCREEN_HEIGHT;
static const int GRID_SQUARE_LENGTH = 24;
