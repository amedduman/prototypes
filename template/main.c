#include "include/raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

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
*/