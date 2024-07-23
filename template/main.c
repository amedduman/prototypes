#include "include/raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

/*
  clear && clang main.c -I include/  -L lib/ -lraylib -framework CoreFoundation -framework IOKit -framework Cocoa && ./a.out

  clear && make && ./game

  make clean

  clang main.c -I include/  -L lib/ -lraylib -framework CoreFoundation -framework IOKit -framework Cocoa

  clang -g main.c -I include/  -L lib/ -lraylib -framework CoreFoundation -framework IOKit -framework Cocoa
*/