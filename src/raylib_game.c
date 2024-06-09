#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

typedef struct {
  Vector3 pos;
  Vector3 size;
} postcard;

int main() {
  const int screenWidth = 400;
  const int screenHeight = 400;
  const int arrayLength = 30;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) {
    SetTargetFPS(60);

    postcard *postcards[arrayLength];
    float radius = 300;
    float centerZ = 300;
    float fl = 300.0;
    float baseAngle = 0;

    while (!WindowShouldClose()) {

      baseAngle += 0.5f * GetFrameTime();

      for (int i = 0; i < arrayLength; i++) {
        // Allocate memory for each postcard
        postcards[i] = (postcard *)malloc(sizeof(postcard));

        float angle = PI * 2 / arrayLength * i + baseAngle;
        float pos_x = cosf(angle) * radius;
        float pos_y = 0;
        float pos_z = sinf(angle) * radius + centerZ;

        int cardSize = 30;
        postcards[i]->pos = (Vector3){pos_x, pos_y, pos_z};
        postcards[i]->size = (Vector3){cardSize, cardSize, cardSize};
      }

      defer(BeginDrawing(), EndDrawing()) {
        ClearBackground(GOLD);

        for (int i = 0; i < arrayLength; i++) {
          float perspective = fl / (fl + postcards[i]->pos.z);
          Vector2 screen_pos = {
              (postcards[i]->pos.x * perspective) + (float)screenWidth / 2,
              (postcards[i]->pos.y * perspective) + (float)screenHeight / 2};
          Vector2 screen_size = {postcards[i]->size.x * perspective,
                                 postcards[i]->size.y * perspective};

          DrawRectangleV(screen_pos, screen_size, BLACK);
        }
      }
    }

    // Free allocated memory
    for (int i = 0; i < arrayLength; i++) {
      free(postcards[i]);
    }
  }
}