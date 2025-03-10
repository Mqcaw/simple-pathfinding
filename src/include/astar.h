#ifndef ASTAR_H
#define ASTAR_H

#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdlib.h>
#include "color.h"
#include "cell.h"
#include "cursor.h"


typedef struct {
    Cell **cells;
    size_t size;
    size_t capacity;
} CellList;

void initCellList(CellList *list, size_t initial_capacity);
void push(CellList *list, Cell* cell);
Cell* pop(CellList *list);
Cell* removeCell(CellList *list, Cell *cell);
void freeCellList(CellList *list);
Cell* findLowestFCost(CellList *list);
int contains(CellList *list, Cell *cell);
int heuristic(Cell* a, Cell* b);
void calculateCosts(Cell* currnet, Cell* start, Cell* end);
void updateNeighbours(Cell** grid, CellList* neighboursList, Cell* cell, const  int* width, const int* height);
void astar(Cursor* cursor, SDL_Renderer *renderer, const Color* colors, Cell **grid, Cell *startCell, Cell *endCell, const int* width, const int* height, const int* rows, const int* cols, const int* spacing);


#endif // ASTAR_H
