@echo off

rem Complie
gcc -I src/include -L src/lib -o src/bin/main src/main.c src/render.c src/astar.c -lmingw32 -lSDL2main -lSDL2


rem Complie
"src/bin/main.exe"