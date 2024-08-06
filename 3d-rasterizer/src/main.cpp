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

/*
void read_and_load_face(const vector<string>& face_data, vector<Vector3>& v, vector<Vector2>& vt, vector<Vector3>& vn, vector<ssr::triangle_new_t>& faces)
{
    ssr::triangle_new_t first_triangle;
    ssr::triangle_new_t second_triangle;
    // first face
    // vertex 1
    // c stands for column
    vector<string> c_1 = string_split(face_data[1], "/");
    first_triangle.v1.postion =  v[std::stoi(c_1[0])];
    first_triangle.v2.uv      = vt[std::stoi(c_1[1])];
    first_triangle.v3.normal  = vn[std::stoi(c_1[2])];

    // vector<string> c_1 = string_split(face_data[1], "/");
    // first_triangle.v1.postion = v[std::stoi(c_1[0])];
    // first_triangle.v2.uv = vt[std::stoi(c_1[1])];
    // first_triangle.v3.normal = vn[std::stoi(c_1[2])];
        // face data looks like this
        f 1/1/1 5/2/1 7/3/1 3/4/1
        f 4/5/2 3/4/2 7/6/2 8/7/2
        f 8/8/3 7/9/3 5/10/3 6/11/3
        f 6/12/4 2/13/4 4/5/4 8/14/4
        f 2/13/5 1/1/5 3/4/5 4/5/5
        f 6/11/6 5/10/6 1/1/6 2/13/6

    for(size_t i = 1; i < face_data.size(); i++)
    {
        // c stands for column
        vector<string> c = string_split(face_data[1], "/");
        first_triangle.v1.postion =  v[std::stoi(c[0])];
        first_triangle.v2.uv      = vt[std::stoi(c[1])];
        first_triangle.v3.normal  = vn[std::stoi(c[2])];
    }

    for (string& index : string_split(face_data[0], "/"))
    {
    }
    // vertex2
    for (auto& index : string_split(words[1], "/"))
    {
        f.push_back(std::stof(index));
    }
    // vertex3
    for (auto& index : string_split(words[2], "/"))
    {
        f.push_back(std::stof(index));
    }
    // second face
    for (auto& index : string_split(words[2], "/"))
    {
        f.push_back(std::stof(index));
    }
    for (auto& index : string_split(words[3], "/"))
    {
        f.push_back(std::stof(index));
    }
    for (auto& index : string_split(words[0], "/"))
    {
        f.push_back(std::stof(index));
    }
    //
    for (size_t i = 1; i < words.size(); i++)
    {
        // cout << words[i] << endl;

        for (auto& s : string_split(words[i], "/"))
        {
            // cout << s << endl;
            f.push_back(std::stof(s));
        }
    }
}
*/

int main(void)
{
    
    // load_obj_data(get_full_path("res/cube.obj"), faces);

    const int screenWidth = 400;
    const int screenHeight = 400;

    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, "Game");

    // ssr::model_t cube = {

    //     .vertices =
    //         {
    //             (Vector3){1, 1, 1},    // 0
    //             (Vector3){-1, 1, 1},   // 1
    //             (Vector3){-1, -1, 1},  // 2
    //             (Vector3){1, -1, 1},   // 3
    //             (Vector3){1, 1, -1},   // 4
    //             (Vector3){-1, 1, -1},  // 5
    //             (Vector3){-1, -1, -1}, // 6
    //             (Vector3){1, -1, -1},  // 7
    //         },
    //     .triangles =
    //         {
    //             {.tri_indices = {0, 1, 2}, .color = RED},
    //             {.tri_indices = {0, 2, 3}, .color = RED},
    //             {.tri_indices = {4, 0, 3}, .color = GREEN},
    //             {.tri_indices = {4, 3, 7}, .color = GREEN},
    //             {.tri_indices = {5, 4, 7}, .color = BLUE},
    //             {.tri_indices = {5, 7, 6}, .color = BLUE},
    //             {.tri_indices = {1, 5, 6}, .color = GOLD},
    //             {.tri_indices = {1, 6, 2}, .color = GOLD},
    //             {.tri_indices = {4, 5, 1}, .color = PURPLE},
    //             {.tri_indices = {4, 1, 0}, .color = PURPLE},
    //             {.tri_indices = {2, 6, 7}, .color = BLACK},
    //             {.tri_indices = {2, 7, 3}, .color = BLACK},
    //         },
    //     .uv_of_each_vertex =
    //         {
    //             (Vector2){1, 0}, // 0
    //             (Vector2){0, 0}, // 1
    //             (Vector2){0, 1}, // 2
    //             (Vector2){0, 1}, // 3
    //             (Vector2){1, 1}, // 4
    //             (Vector2){0, 1}, // 5
    //             (Vector2){0, 0}, // 6
    //             (Vector2){1, 0}, // 7
    //         },
    //     .colors =
    //         {
    //             BLACK,
    //             BLACK,
    //             BLACK,
    //             BLACK,
    //             RED,
    //             GREEN,
    //             PURPLE,
    //             BLACK,
    //         }

    // };

    // ssr::instance_t cube_a = ssr::model_init_instance(cube,
    //                                                   (Vector3){0, 0, 7},
    //                                                   (Vector3){0, DEG2RAD * 0, 0},
    //                                                   (Vector3){1, 1, 1});
    // ssr::instance_t cube_b = model_init_instance(cube,
    //                                              (Vector3){0, 0, 3},
    //                                              (Vector3){0, DEG2RAD * 30, 0},
    //                                              (Vector3){1, 1, 1});

    ssr::camera_t camera = ssr::camera_t({0, 0, 0},
                                         (Vector3){0, DEG2RAD * 0, 0},
                                         90,
                                         0.1f,
                                         300.0f);

    ssr::Renderer renderer = ssr::Renderer();

    model_loader m;
    ssr::model_new model = m.load_obj_data(get_full_path("res/cube.obj"));

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        camera.move();
        camera.rotate();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        renderer.render_scene(vector<ssr::model_new>{model}, camera);

        EndDrawing();
    }

    CloseWindow();
}