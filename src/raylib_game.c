#include "defer.h"
#include "utils.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

void DragCircle(Vector2 *circleCenter, float radius)
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (ut_IsPointCircleIntersect(GetMousePosition(), *circleCenter, radius))
        {
            *circleCenter = GetMousePosition();
        }
    }

    /*
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (ut_IsPointCircleIntersect(GetMousePosition(), *circleCenter, radius))
        {
            *is_holding_handle = true;
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (*is_holding_handle)
        {
            *circleCenter = GetMousePosition();
        }
    }
    else
    {
        *is_holding_handle = false;
    }
    */
}

int main()
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 p0 = {100, 100};
        Vector2 p1 = {200, 100};
        Vector2 p2 = {300, 200};
        Vector2 p3 = {300, 300};
        float radius = 5;

        bool is_holding_handle = false;
        while (!WindowShouldClose())
        {
            DragCircle(&p0, radius);
            DragCircle(&p1, radius);
            DragCircle(&p2, radius);
            DragCircle(&p3, radius);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                for (float t = 0; t < 1; t += 0.01f)
                {
                    Vector2 circleCenter = ut_cubicBezier(p0, p1, p2, p3, t);
                    DrawCircleLinesV(circleCenter, 3, RED);
                }

                DrawCircleV(p0, radius, WHITE);
                DrawCircleV(p1, radius, WHITE);
                DrawCircleV(p2, radius, WHITE);
                DrawCircleV(p3, radius, WHITE);
            }
        }
    }
}