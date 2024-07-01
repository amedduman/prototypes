#include "include/raylib.h"
#include "src/circle.h"
#include "src/matrix.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    my_matrix m;
    my_matrix_init(&m, 2, 3);
    my_matrix_print(&m);
    float val = my_matrix_get_val(&m, 1, 1);
    printf("the (1,1) values is %f\n", val);
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