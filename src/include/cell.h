#ifndef CELL_H
#define CELL_H

#include "color.h"

typedef struct Cell{
    int walkable;
    int gCost;
    int hCost;
    int fCost; 
    int x;
    int y;
    int isStartCell;
    int isEndCell;
    int isPath;
    int isClosed;
    int isOpen;
    int visited;
    struct Cell *parent;
} Cell;

#endif // CELL_H