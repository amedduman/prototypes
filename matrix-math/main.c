#include "include/raylib.h"
#include "src/circle.h"
#include "src/matrix.h"
#include <stdio.h>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    my_matrix m1;
    my_matrix_init(&m1, 2, 3);
    my_matrix_randomize(&m1);
    my_matrix_print(&m1);
    printf("------------------\n");
    my_matrix m2;
    my_matrix_init(&m2, 2, 3);
    my_matrix_randomize(&m2);
    my_matrix_print(&m2);

    printf("------------------\n");
    my_matrix mul_matrix = my_matrix_mul_with_matrix(&m1, &m2);
    my_matrix_print(&mul_matrix);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawMyCircle();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}