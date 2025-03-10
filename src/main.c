#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "color.h"
#include "cursor.h"
#include "cell.h"
#include "render.h"

#undef main

const int WIDTH = 800;
const int HEIGHT = 800;
const int ROWS = 50;
const int COLS = 50;
const int SPACING = 2;




const Color COLORS[6] = {         
    {217, 132, 108, 255},          // Unwalkable 217, 132, 108
    {60, 60, 60, 255},    // Walkable
    {0, 0, 0, 255},          //Background
    {0, 255, 0, 255},       // Start 148,192,142
    {224,93,99, 255},        // End 
    {227, 187, 41, 255}      // Path
};

Cursor cursor = {
    0, 0, 1, {85,168,163, 220}
};




void clearStartCells(Cell **grid, const int* rows, const int * cols);
void clearEndCells(Cell **grid, const int* rows, const int * cols);
void generateMaze(Cell **grid, int row, int col, int rows, int cols);
void initializeMaze(Cell **grid, int rows, int cols);



void clearStartCells(Cell **grid, const int* rows, const int * cols) {
    for (int row = 0; row < *rows; row++) {
        for (int col = 0; col < *cols; col++) {
            grid[row][col].isStartCell = 0;
        }
    }
}

void clearEndCells(Cell **grid, const int* rows, const int * cols) {
    for (int row = 0; row < *rows; row++) {
        for (int col = 0; col < *cols; col++) {
            grid[row][col].isEndCell = 0;
        }
    }
}


void generateMaze(Cell **grid, int row, int col, int rows, int cols) {
    // Direction vectors for moving in 4 directions
    int directions[4][2] = {
        {0, 1}, // Right
        {1, 0}, // Down
        {0, -1}, // Left
        {-1, 0} // Up
    };

    // Shuffle directions to ensure randomness
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp[2] = {directions[i][0], directions[i][1]};
        directions[i][0] = directions[r][0];
        directions[i][1] = directions[r][1];
        directions[r][0] = temp[0];
        directions[r][1] = temp[1];
    }

    for (int i = 0; i < 4; i++) {
        int newRow = row + directions[i][0] * 2;
        int newCol = col + directions[i][1] * 2;

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && grid[newRow][newCol].visited == 0) {
            grid[newRow][newCol].walkable = 1;
            grid[row + directions[i][0]][col + directions[i][1]].walkable = 1;
            grid[newRow][newCol].visited = 1;

            generateMaze(grid, newRow, newCol, rows, cols);
        }
    }
}


void initializeMaze(Cell **grid, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            grid[row][col].walkable = 0;
            grid[row][col].visited = 0;
        }
    }

    int startRow = rand() % rows;
    int startCol = rand() % cols;

    grid[startRow][startCol].walkable = 1;
    grid[startRow][startCol].visited = 1;

    generateMaze(grid, startRow, startCol, rows, cols);
}




int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Path Finding", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_ShowCursor(SDL_DISABLE);

    init(renderer, COLORS);

    Cell **grid = (Cell **)malloc(ROWS * sizeof(Cell *));
    for (int i = 0; i < ROWS; ++i) {
        grid[i] = (Cell *)malloc(COLS * sizeof(Cell));
        if (grid[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        for (int j = 0; j < COLS; ++j) {
            grid[i][j].walkable = 1;
            grid[i][j].gCost = 0;
            grid[i][j].hCost = 0;
            grid[i][j].fCost = 0;
            grid[i][j].x = j;
            grid[i][j].y = i;
            grid[i][j].isStartCell = 0;
            grid[i][j].isEndCell = 0;
            grid[i][j].isPath = 0;
            grid[i][j].isOpen = 0;
            grid[i][j].isClosed = 0;
            grid[i][j].visited = 0;
            grid[i][j].parent = NULL;
        }
    }
    Cell** newGrid = grid;

    render(renderer, grid, &cursor, COLORS, &WIDTH, &HEIGHT, &ROWS, &COLS, &SPACING);

    SDL_Event event;
    int running = 1;
    int paused = 1;
    int ctrlPressed = 0;
    int leftMouseDown = 0;
    int rightMouseDown = 0;
    Cell* startCell = NULL;
    Cell* endCell = NULL;

    while (running) {
        render(renderer, grid, &cursor, COLORS, &WIDTH, &HEIGHT, &ROWS, &COLS, &SPACING);

        while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_LCTRL) {
                    ctrlPressed = 1;
                } else if (event.key.keysym.sym == SDLK_m) {
                    initializeMaze(grid, ROWS, COLS);
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    paused = !paused;
                } else if (event.key.keysym.sym == SDLK_r) {
                    randomizeGrid(grid, &ROWS, &COLS);
                } else if (event.key.keysym.sym == SDLK_c) {
                    resetGrid(grid, &ROWS, &COLS);
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    updateGrid(&cursor, renderer, grid, COLORS, newGrid, &ROWS, &COLS, &SPACING, &WIDTH, &HEIGHT, startCell, endCell);
                    Cell **temp = grid;
                    grid = newGrid;
                    newGrid = temp;
                }
                render(renderer, grid, &cursor, COLORS, &WIDTH, &HEIGHT, &ROWS, &COLS, &SPACING);
                break;

            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_LCTRL) {
                    ctrlPressed = 0;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (ctrlPressed && event.button.button == SDL_BUTTON_LEFT) {
                    clearStartCells(grid, &ROWS, &COLS);
                    grid[cursor.y][cursor.x].isStartCell = 1;
                    startCell = &grid[cursor.y][cursor.x];
                } else if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseDown = 1;
                }

                if (ctrlPressed && event.button.button == SDL_BUTTON_RIGHT) {
                    clearEndCells(grid, &ROWS, &COLS);
                    grid[cursor.y][cursor.x].isEndCell = 1;
                    endCell = &grid[cursor.y][cursor.x];
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    rightMouseDown = 1;
                }
                
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseDown = 0;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    rightMouseDown = 0;
                }

            }
        }

        if (leftMouseDown && rightMouseDown) {
            continue;
        } else if (rightMouseDown) {
            grid[cursor.y][cursor.x].walkable = 1;
        } else if (leftMouseDown) {
            grid[cursor.y][cursor.x].walkable = 0;
        }
        

        if (!paused) {
            updateGrid(&cursor, renderer, grid, COLORS, newGrid, &ROWS, &COLS, &SPACING, &WIDTH, &HEIGHT, startCell, endCell);

            Cell **temp = grid;
            grid = newGrid;
            newGrid = temp;

            render(renderer, grid, &cursor, COLORS, &WIDTH, &HEIGHT, &ROWS, &COLS, &SPACING);

            SDL_Delay(50);
        }
    }

    for (int i = 0; i < COLS; ++i) {
        free(grid[i]);
        free(newGrid[i]);
    }
    free(grid);
    free(newGrid);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}