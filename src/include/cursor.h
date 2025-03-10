#ifndef CURSOR_H
#define CURSOR_H

#include "color.h"
#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    int thickness;
    Color color;
    SDL_Rect cell;
} Cursor;

#endif // CURSOR_H