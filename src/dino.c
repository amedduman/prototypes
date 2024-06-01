# include "dino.h"
#include <stdio.h>
#include <raymath.h>

void InitDino(Dino* dino)
{
    dino->image = LoadTexture("./src/resources/dino.png");
    dino->image.width = 100;
    dino->image.height = 100;
    dino->position = (Vector2){0,0};
    dino->rect = (Rectangle){dino->position.x, dino->position.y, (float)dino->image.width, (float)dino->image.height};
    dino->speed = 5;
}

void FreeDinoRes(Dino *dino)
{
    UnloadTexture(dino->image);
}

void Update(Dino* dino)
{
    Vector2 velocity = {0, 0};
    if (IsKeyDown(KEY_RIGHT)) velocity.x += 1;
    else if(IsKeyDown(KEY_LEFT)) velocity.x -= 1;
    if(IsKeyDown(KEY_UP)) velocity.y -= 1;
    else if(IsKeyDown(KEY_DOWN)) velocity.y += 1;
    velocity = Vector2Normalize(velocity);
    velocity = Vector2Scale(velocity, dino->speed);
    dino->position = Vector2Add(dino->position, velocity);
    dino->rect.x = dino->position.x;
    dino->rect.y = dino->position.y;
}

void Draw(Dino* dino)
{
    DrawTextureV(dino->image, dino->position, WHITE);
}

void DrawRect(Dino *dino, bool isColliding)
{
    if(isColliding)
        DrawRectangleLinesEx(dino->rect, 5, MAROON);
    else
        DrawRectangleLinesEx(dino->rect, 5, PURPLE);
}
