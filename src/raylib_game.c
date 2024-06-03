#include "defer.h"
#include "raylib.h"
#include "3dMath.h"
#include <stdio.h>

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Game"), CloseWindow())
    {
        SetTargetFPS(24);

        MyMesh mesh_cube;
        mesh_cube.tris = vector_create();

        Tri south1 = {{ {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0} }};
        Tri south2 = {{ {0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0} }};
        vector_add(&mesh_cube.tris, south1);
        vector_add(&mesh_cube.tris, south2);

        Tri east1 = {{ {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0} }};
        Tri east2 = {{ {1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0} }};
        vector_add(&mesh_cube.tris, east1);
        vector_add(&mesh_cube.tris, east2);

        Tri north1 = {{ {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0} }};
        Tri north2 = {{ {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 0.0, 1.0} }};
        vector_add(&mesh_cube.tris, north1);
        vector_add(&mesh_cube.tris, north2);

        Tri west1 = {{ {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 0.0} }};
        Tri west2 = {{ {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0} }};
        vector_add(&mesh_cube.tris, west1);
        vector_add(&mesh_cube.tris, west2);

        Tri top1 = {{ {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0} }};
        Tri top2 = {{ {0.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 0.0} }};
        vector_add(&mesh_cube.tris, top1);
        vector_add(&mesh_cube.tris, top2);

        Tri bottom1 = {{ {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0} }};
        Tri bottom2 = {{ {1.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0} }};
        vector_add(&mesh_cube.tris, bottom1);
        vector_add(&mesh_cube.tris, bottom2);

        bool did = false;

        while (!WindowShouldClose())
        {
            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);

                if(did == false)
                {
                    for (int i = 0; i < vector_size(mesh_cube.tris); i++)
                    {
                        printf("%d \n", i);
                    }

                    did = true;
                }
            }
        }

        vector_free(mesh_cube.tris);
    }
}













/* 
#include "defer.h"
#include "raylib.h"
#include "3dMath.h"

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Game"), CloseWindow())
    {
        SetTargetFPS(24);

        MyMesh mesh_cube;
        mesh_cube.tris = vector_create();

        Tri south1 = {{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}};
        Tri south2 = {{0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}};
        vector_add(&mesh_cube.tris, south1);
        vector_add(&mesh_cube.tris, south2);

        Tri east1 = {{1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};
        Tri east2 = {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};
        vector_add(&mesh_cube.tris, east1);
        vector_add(&mesh_cube.tris, east1);

        Tri north1 = {{1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
        Tri north2 = {{1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}};
        vector_add(&mesh_cube.tris, north1);
        vector_add(&mesh_cube.tris, north2);

        Tri west1 = {{0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 0.0}};
        Tri west2 = {{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}};
        vector_add(&mesh_cube.tris, west1);
        vector_add(&mesh_cube.tris, west2);

        Tri top1 = {{0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
        Tri top2 = {{0.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 0.0}};
        vector_add(&mesh_cube.tris, top1);
        vector_add(&mesh_cube.tris, top2);

        Tri bottom1 = {{1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}};
        Tri bottom2 = {{1.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};
        vector_add(&mesh_cube.tris, bottom1);
        vector_add(&mesh_cube.tris, bottom2);


        while (!WindowShouldClose())
        {
            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);
            }
        }

        vector_free(mesh_cube.tris);
    }
} */