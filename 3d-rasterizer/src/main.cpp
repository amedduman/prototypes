#include "../include/raylib.h"
#include "../include/raymath.h"
#include "model_loader.h"
#include "rendering.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
namespace fs = std::filesystem;

string get_full_path(const string& relative_path_str)
{
    fs::path current_path = fs::current_path();
    fs::path relative_path = relative_path_str;
    fs::path full_path = current_path / relative_path;

    return full_path.string();
}

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, "Game");

    ssr::camera_t camera = ssr::camera_t({0, 0, 0},
                                         (Vector3){0, DEG2RAD * 0, 0},
                                         90,
                                         0.1f,
                                         300.0f);

    ssr::Renderer renderer = ssr::Renderer();

    ssr::model_loader m;
    ssr::model_t model = m.load_obj_data(get_full_path("res/cube.obj"));

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        camera.move();
        camera.rotate();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        renderer.render_scene(vector<ssr::model_t>{model}, camera);

        EndDrawing();
    }

    CloseWindow();
}