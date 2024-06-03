#pragma once 
#include "vec.h"

typedef struct Vec3
{
    float x;
    float y;
    float z;
}Vec3;

typedef struct Tri
{
    Vec3 p[3];
}Tri;

typedef struct MyMesh
{
    Tri* tris;
}MyMesh;
