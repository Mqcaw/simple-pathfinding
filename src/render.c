#include "render.h"
#include "astar.h"


SDL_Renderer* init(SDL_Renderer* renderer, const Color* colors) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, colors[1].r, colors[1].g, colors[1].b, colors[1].a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return renderer;
}


void updateGrid(Cursor* cursor, SDL_Renderer *renderer, Cell **grid, const Color* colors, Cell **newGrid, const int* rows, const int* cols, const int* spacing, const int* width, const int* height, Cell *startCell, Cell *endCell) {
    if (!startCell && !endCell) {
        return;
    }

    astar(cursor, renderer, colors, grid, startCell, endCell, width, height, rows, cols, spacing);
}



void drawGrid(SDL_Renderer *renderer, Cell **grid, const Color* colors, const int* width, const int* height, const int* rows, const int* cols, const int* spacing) {
    int cellWidth = (*width) / *cols;
    int cellHeight = (*height) / *rows;

    for (int row = 0; row < *rows; ++row) {
        for (int col = 0; col < *cols; ++col) {
            if (grid[row][col].isStartCell) {
                SDL_SetRenderDrawColor(renderer, colors[3].r, colors[3].g, colors[3].b, colors[3].a);
            } else if (grid[row][col].isEndCell) {
                SDL_SetRenderDrawColor(renderer, colors[4].r, colors[4].g, colors[4].b, colors[4].a);
            } else {
                SDL_SetRenderDrawColor(renderer, colors[grid[row][col].walkable].r, colors[grid[row][col].walkable].g, colors[grid[row][col].walkable].b, colors[grid[row][col].walkable].a);
            }
            
            SDL_Rect cell = {col * cellWidth, row * cellHeight, cellWidth - *spacing, cellHeight - *spacing};
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

void drawPath(SDL_Renderer* renderer, Cell **grid, const Color* colors, const int* width, const int* height, const int* rows, const int* cols, const int* spacing) {
    int cellWidth = (*width) / *cols;
    int cellHeight = (*height) / *rows;

    for (int row = 0; row < *rows; ++row) {
        for (int col = 0; col < *cols; ++col) {
            if (grid[row][col].isPath) {
                SDL_SetRenderDrawColor(renderer, colors[5].r, colors[5].g, colors[5].b, colors[5].a);
                SDL_Rect cell = {col * cellWidth, row * cellHeight, cellWidth - *spacing, cellHeight - *spacing};
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
}

void drawCursor(SDL_Renderer* renderer, Cursor* cursor, const int* width, const int* height, const int* rows, const int* cols, const int* spacing) {
    int cursorWidth = *width / *cols;
    int cursorHeight = *height / *rows;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int gridX = mouseX / cursorWidth;
    int gridY = mouseY / cursorHeight;

    cursor->x = gridX;
    cursor->y = gridY;

    int screenX = gridX * cursorWidth;
    int screenY = gridY * cursorHeight;

    cursor->cell.x = screenX;
    cursor->cell.y = screenY;
    cursor->cell.w = cursorWidth - *spacing;
    cursor->cell.h = cursorHeight - *spacing;

    SDL_SetRenderDrawColor(renderer, cursor->color.r, cursor->color.g, cursor->color.b, cursor->color.a);
    SDL_RenderFillRect(renderer, &cursor->cell);
}


void render(SDL_Renderer *renderer, Cell **grid, Cursor* cursor, const Color* colors, const int* width, const int* height, const int* rows, const int* cols, const int* spacing) {
    //main rendering logic

    //set background color to white and clear the screen
    SDL_SetRenderDrawColor(renderer, colors[2].r, colors[2].g, colors[2].b, colors[2].a);
    SDL_RenderClear(renderer);

    //draw the grid and the cursor
    drawGrid(renderer, grid, colors, width, height, rows, cols, spacing);
    drawPath(renderer, grid, colors, width, height, rows, cols, spacing);
    drawCursor(renderer, cursor, width, height, rows, cols, spacing);

    //present the rendered screen
    SDL_RenderPresent(renderer);
}

void randomizeGrid(Cell **grid, const int* rows, const int* cols) {
    resetGrid(grid, rows, cols);
    for (int row = 0; row < *rows; row++) {
        for (int col = 0; col < *cols; col++) {
            grid[row][col].walkable = rand() % 2;
        }
    }
}

void resetGrid(Cell **grid, const int* rows, const int* cols) {
    for (int row = 0; row < *rows; row++) {
        for (int col = 0; col < *cols; col++) {
            grid[row][col].walkable = 1;
            grid[row][col].gCost = 0;
            grid[row][col].hCost = 0;
            grid[row][col].fCost = 0;
            grid[row][col].x = col;
            grid[row][col].y = row;
            grid[row][col].isStartCell = 0;
            grid[row][col].isEndCell = 0;
            grid[row][col].isPath = 0;
            grid[row][col].isOpen = 0;
            grid[row][col].isClosed = 0;
            grid[row][col].parent = NULL;
        }
    }
}

void fillGrid(Cell **grid, const int* rows, const int* cols) {
    for (int row = 0; row < *rows; row++) {
        for (int col = 0; col < *cols; col++) {
            grid[row][col].walkable = 0;
            grid[row][col].gCost = 0;
            grid[row][col].hCost = 0;
            grid[row][col].fCost = 0;
            grid[row][col].x = col;
            grid[row][col].y = row;
            grid[row][col].isStartCell = 0;
            grid[row][col].isEndCell = 0;
            grid[row][col].isPath = 0;
            grid[row][col].isOpen = 0;
            grid[row][col].isClosed = 0;
            grid[row][col].parent = NULL;
        }
    }
}
