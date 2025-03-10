#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "astar.h"
#include "render.h"
#include "cursor.h"

void initCellList(CellList *list, size_t initial_capacity) {
    list->cells = (Cell **)malloc(initial_capacity * sizeof(Cell *));
    if (list->cells == NULL) {
        printf("Failed to allocate memory\n");
        exit(EXIT_FAILURE); // Exit if memory allocation fails
    }
    list->size = 0;
    list->capacity = initial_capacity;
}


void push(CellList *list, Cell *cell) {
    if (list->size >= list->capacity) {
        // Resize the list
        list->capacity *= 2;
        list->cells = realloc(list->cells, list->capacity * sizeof(Cell*));
        
    }
    list->cells[list->size++] = cell; 
}



Cell* pop(CellList *list) {
    if (list->size == 0) {
        printf("Pop failure, list is empty\n");
        return NULL;
    }
    return list->cells[--list->size];
}

Cell* removeCell(CellList *list, Cell *cell) {
    size_t index = (size_t)-1; // Use a valid size_t value for initialization

    for (size_t i = 0; i < list->size; i++) {
        if (list->cells[i] == cell) {
            index = i;
            break;
        }
    }

    if (index == (size_t)-1) {
        printf("Remove Error, Cell not found\n");
        return NULL;
    }

    // Shift elements to the left to fill the gap
    for (size_t i = index; i < list->size - 1; i++) {
        list->cells[i] = list->cells[i + 1];
    }

    list->size--;
    return cell;
}

void freeCellList(CellList *list) {
    if (list->cells != NULL) {
        free(list->cells);
        list->cells = NULL; // Ensure dangling pointer is cleared
    }
}



int contains(CellList *list, Cell *cell) {
    for (int i = 0; i < list->size; i++) {
        if (list->cells[i] == cell) {
            return 1;
        }
    }
    return 0;
}

int heuristic(Cell* a, Cell* b) {
    int dx = abs(b->x - a->x);
    int dy = abs(b->y - a->y);
    return 10 * (dx + dy) + (14 - 2 * 10) * fmin(dx, dy);
}

void calculateCosts(Cell* current, Cell* start, Cell* end) {
    current->gCost = heuristic(start, current); 
    current->hCost = heuristic(current, end);
    current->fCost = current->gCost + current->hCost;
}


void updateNeighbours(Cell** grid, CellList* neighboursList, Cell* cell, const int* width, const int* height) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue; // Skip the current cell
            }
            int newX = cell->x + j;
            int newY = cell->y + i;
            if (newX >= 0 && newY >= 0 && newX < *width && newY < *height) {
                push(neighboursList, &grid[newY][newX]);
            }
        }
    }
}


void astar(Cursor* cursor, SDL_Renderer *renderer, const Color* colors, Cell **grid, Cell *startCell, Cell *endCell, const int* width, const int* height, const int* rows, const int* cols, const int* spacing) {
    CellList open;
    CellList close;
    CellList neighbours;
    CellList path;
    initCellList(&open, 2);
    initCellList(&close, 2);
    initCellList(&neighbours, 8);
    initCellList(&path, 1);

    calculateCosts(startCell, startCell, endCell);
    push(&open, startCell);


    while (open.size != 0) {
        Cell* currentCell = open.cells[0];
        for (size_t i = 0; i < open.size; i++) {
            if (open.cells[i]->fCost < currentCell->fCost || open.cells[i]->fCost == currentCell->fCost && open.cells[i]->hCost < currentCell->hCost) {
                currentCell = open.cells[i];
            }
        }
        removeCell(&open, currentCell);
        push(&close, currentCell);
        currentCell->isClosed = 1;
        if (currentCell == endCell) {
            Cell* currentBackCell = endCell;
            while (currentBackCell != NULL) {
                push(&path, currentBackCell);
                currentBackCell->isPath = 1;
                currentBackCell = currentBackCell->parent;
            }
            break;
        }
        neighbours.size = 0;
        updateNeighbours(grid, &neighbours, currentCell, width, height);

        for (size_t i = 0; i < neighbours.size; i++) {
            Cell* neighbourCell = neighbours.cells[i];
            if (neighbourCell == NULL || contains(&close, neighbourCell) || neighbourCell->walkable == 0) {
                continue;
            }

            int tentativeGCost = currentCell->gCost + heuristic(currentCell, neighbourCell);
            if (tentativeGCost < neighbourCell->gCost || !contains(&open, neighbourCell)) {
                neighbourCell->gCost = tentativeGCost;
                neighbourCell->hCost = heuristic(neighbourCell, endCell);
                neighbourCell->parent = currentCell;
                if (!contains(&open, neighbourCell)) {
                    push(&open, neighbourCell);
                    neighbourCell->isOpen = 1;
                }
            }
            neighbours.cells[i] = NULL;
            //render(renderer, grid, cursor, colors, width, height, rows, cols, spacing);
        }

        if (currentCell != endCell && open.size == 0) {
            printf("No Solution Found");
            break;
        }


    }


    freeCellList(&open);
    freeCellList(&close);
    freeCellList(&neighbours);
    freeCellList(&path);
}