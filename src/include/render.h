#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "cell.h"
#include "color.h"
#include "cursor.h"


SDL_Renderer* init(SDL_Renderer* renderer, const Color* colors);
void updateGrid(Cursor* cursor, SDL_Renderer *renderer, Cell **grid, const Color* colors, Cell **newGrid, const int* rows, const int* cols, const int* spacing, const int* width, const int* height, Cell *startCell, Cell *endCell);
void drawGrid(SDL_Renderer *renderer, Cell **grid, const Color* colors, const int* width, const int* height, const int* rows, const int* cols, const int* spacing);
void drawPath(SDL_Renderer* renderer, Cell **grid, const Color* pathColor, const int* width, const int* height, const int* rows, const int* cols, const int* spacing);
void drawCursor(SDL_Renderer* renderer, Cursor* cursor, const int* width, const int* height, const int* rows, const int* cols, const int* spacing) ;
void render(SDL_Renderer *renderer, Cell **grid, Cursor* cursor, const Color* colors, const int* width, const int* height, const int* rows, const int* cols, const int* spacing);
void randomizeGrid(Cell **grid, const int* rows, const int* cols);
void resetGrid(Cell **grid, const int* rows, const int* cols);




#endif //RENDER_H