#pragma once
#include <raylib.h>
#include <raymath.h>

float ut_Rndf(float min, float max)
{
    return ((float)GetRandomValue(0, 10000) / 10000.0f) * (max - min) + min;
}

Vector2 ut_Vector2MulVal(Vector2 vec, float val)
{
    return (Vector2){vec.x * val, vec.y * val};
}

bool ut_IsPointCircleIntersect(Vector2 point, Vector2 center, float radius)
{
    return Vector2DistanceSqr(center, point) < radius * radius;
}