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

    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        postcard *postcards[10];
        float card_z = 500;
        for (int i = 0; i < 10; i++)
        {
            // Allocate memory for each postcard
            postcards[i] = (postcard *)malloc(sizeof(postcard));

            int rnd_pos_x = GetRandomValue(0, screenWidth);
            int rnd_pos_y = GetRandomValue(0, screenHeight);
            int rnd_size_x = GetRandomValue(20, 100);
            int rnd_size_y = GetRandomValue(20, 20);

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

                for (int i = 0; i < 10; i++)
                {
                    Vector2 screen_pos = {postcards[i]->pos.x * perspective, postcards[i]->pos.y * perspective};
                    Vector2 screen_size = {postcards[i]->size.x * perspective, postcards[i]->size.y * perspective};

                    DrawRectangleV(screen_pos, screen_size, DARKBLUE);
                }
            }
        }

        // Free allocated memory
        for (int i = 0; i < 10; i++)
        {
            free(postcards[i]);
        }
    }
}