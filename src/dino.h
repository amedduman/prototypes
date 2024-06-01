#pragma once
#include <raylib.h>

typedef struct Dino
{
    Texture2D image;
    Vector2 position;
    Rectangle rect;
    float speed;
}Dino;

void InitDino(Dino* dino);
void FreeDinoRes(Dino* dino);
void Update(Dino* dino);
void Draw(Dino* dino);
void DrawRect(Dino* dino, bool isColliding);

