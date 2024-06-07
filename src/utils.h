#pragma once
#include <raylib.h>

float ut_rndf(float min, float max)
{
    return ((float)GetRandomValue(0, 10000) / 10000.0f) * (max - min) + min;
}

Vector2 ut_Vector2MulVal(Vector2 vec, float val)
{
    return (Vector2){vec.x * val, vec.y * val};
}