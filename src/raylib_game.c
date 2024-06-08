#include "defer.h"
#include "utils.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

void DragCircle(Vector2 *circleCenter, float radius)
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (ut_IsPointCircleIntersect(GetMousePosition(), *circleCenter, radius + radius))
        {
            *circleCenter = GetMousePosition();
        }
    }
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
        Vector2 cp = {0,0};
        float radius = 5;

        while (!WindowShouldClose())
        {
            DragCircle(&p0, radius);
            DragCircle(&p1, radius);
            DragCircle(&p2, radius);
            DragCircle(&p3, radius);
            
            /*
            cp.x = p1.x * 2 - (p0.x + p2.x) / 2;
	        cp.y = p1.y * 2 - (p0.y + p2.y) / 2;
            */
            cp.x = (p0.x + 2 * p1.x + p2.x) / 4;
            cp.y = (p0.y + 2 * p1.y + p2.y) / 4;


            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                for (float t = 0; t < 1; t += 0.01f)
                {
                    //Vector2 circleCenter = ut_cubicBezier(p0, p1, p2, p3, t);
                    Vector2 circleCenter = ut_QuadraticBezier(p0, p1, p2, t);
                    DrawCircleLinesV(circleCenter, 3, RED);
                }

                DrawCircleV(p0, radius, WHITE);
                DrawCircleV(p1, radius, GRAY);
                DrawCircleV(p2, radius, WHITE);
                DrawCircleV(p3, radius, WHITE);
                DrawCircleV(cp, radius, RED);
            }
        }
    }
}