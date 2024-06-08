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

Vector2 ut_QuadraticBezier(Vector2 p0, Vector2 p1, Vector2 p2, float t)
{
    Vector2 pFinal = {0,0};
    
    pFinal.x = powf(1 - t, 2) * p0.x + 
                (1 - t) * 2 * t * p1.x + 
                t * t * p2.x;

    pFinal.y = powf(1 - t, 2) * p0.y + 
                (1 - t) * 2 * t * p1.y + 
                t * t * p2.y;

	return pFinal;
}

Vector2 cubicBezier(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t)
{
    Vector2 pFinal = {0,0};

    pFinal.x = powf(1 - t, 3) * p0.x + 
                powf(1 - t, 2) * 3 * t * p1.x + 
                (1 - t) * 3 * t * t * p2.x + 
                t * t * t * p3.x;
                
    pFinal.y = powf(1 - t, 3) * p0.y + 
                powf(1 - t, 2) * 3 * t * p1.y + 
                (1 - t) * 3 * t * t * p2.y + 
                t * t * t * p3.y;

    return pFinal;
}
