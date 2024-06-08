#include "defer.h"
#include "utils.h"
#include <raylib.h>
#include <raymath.h>

int main()
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 p0 = {100,100};
        Vector2 p1 = {200,100};
        Vector2 p2 = {300,200};
        Vector2 p3 = {300,300};

        bool is_holding_handle = false;
        while (!WindowShouldClose())
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (ut_IsPointCircleIntersect(GetMousePosition(), p1, 5))
                {
                    is_holding_handle = true;                    
                }                
            }
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if(is_holding_handle)
                {
                    p1 = GetMousePosition();
                }
            }
            else
            {
                is_holding_handle = false;
            }
            

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                for (float t = 0; t < 1; t += 0.01f)
                {
                    Vector2 circleCenter = ut_QuadraticBezier(p0, p1, p2, t);
                    DrawCircleLinesV(circleCenter, 3, RED);
                }
                

                DrawCircleV(p0, 5, WHITE);
                DrawCircleV(p1, 5, WHITE);
                DrawCircleV(p2, 5, WHITE);
                DrawCircleV(p3, 5, WHITE);
            }
        }
    }
}