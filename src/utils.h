#pragma once
#include <raylib.h>

int utrndf(float min, float max)
{
    return ((float)GetRandomValue(0, 10000) / 10000.0f) * (max - min) + min;
}