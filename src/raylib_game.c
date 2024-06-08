#include "defer.h"
#include "utils.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

typedef struct
{
    Vector3 pos;
    Vector3 size;
} postcard;

int main()
{
    const int screenWidth = 400;
    const int screenHeight = 400;
    const int arrayLength = 30;

    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        postcard *postcards[arrayLength];
        float card_z = 500;
        for (int i = 0; i < arrayLength; i++)
        {
            // Allocate memory for each postcard
            postcards[i] = (postcard *)malloc(sizeof(postcard));

            int rnd_pos_x = GetRandomValue(-screenWidth / 2, screenWidth / 2);
            int rnd_pos_y = GetRandomValue(-screenHeight / 2, screenHeight / 2);
            int rnd_size_x = GetRandomValue(20, 100);
            int rnd_size_y = GetRandomValue(20, 100);

            postcards[i]->pos = (Vector3){rnd_pos_x, rnd_pos_y, card_z};
            postcards[i]->size = (Vector3){rnd_size_x, rnd_size_y, 50};
        }

        float fl = 300.0;

        while (!WindowShouldClose())
        {
            /*
            screen_pos = (Vector2){obj_pos.x * perspective, obj_pos.y * perspective};
            screen_size = (Vector2){obj_size.x * perspective, obj_pos.y * perspective};

            */

            float perspective = fl / (fl + card_z);
            card_z -= 5 * GetFrameTime() * 10;
            if (card_z < 0)
            {
                card_z = 500;
            }

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                for (int i = 0; i < arrayLength; i++)
                {
                    Vector2 screen_pos = {
                        (postcards[i]->pos.x * perspective) + screenWidth / 2,
                        (postcards[i]->pos.y * perspective) + screenHeight / 2
                        };
                    Vector2 screen_size = {postcards[i]->size.x * perspective, postcards[i]->size.y * perspective};

                    DrawRectangleV(screen_pos, screen_size, DARKBLUE);
                }
            }
        }

        // Free allocated memory
        for (int i = 0; i < arrayLength; i++)
        {
            free(postcards[i]);
        }
    }
}