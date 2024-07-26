#include "include/raylib.h"
#include "include/raymath.h"
#include <iostream>

#pragma region log

template <typename T>
void logx(const T& msg)
{
  using namespace std;
  cout << msg << endl;
}

#pragma endregion

// Operator overloading for Vector2 addition
Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2Add(v1, v2);
}

// Operator overloading for Vector2 output
std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    Vector2 x = {1,2};
    Vector2 y = {2,3};
    Vector2 sum = x + y;
    
    logx(sum);

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