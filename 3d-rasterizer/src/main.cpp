#include <assert.h>

#include "../include/raylib.h"
#include "../include/raymath.h"
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

vector<string> string_split(const string& str, const string& delimeter)
{
    vector<string> result;
    // size_t pos = 0;
    size_t previos_index = 0;

    // TODO: replace this.
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == delimeter[0])
        {
            result.push_back(str.substr(previos_index, i - previos_index));
            previos_index = i + 1;
        }
    }
    return result;
    /*
    while (pos != string::npos)
    {
        pos = str.find(delimeter);


        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delimeter.length());
    }

    return result;
    */
}

string get_full_path(const string& relative_path_str)
{
    fs::path current_path = fs::current_path();
    fs::path relative_path = relative_path_str;
    fs::path full_path = current_path / relative_path;

    return full_path.string();
}

Vector3 read_vec3(const vector<string>& words)
{
    return (Vector3){std::stof(words[1]), std::stof(words[2]), std::stof(words[3])};
}

Vector2 read_vec2(const vector<string>& words)
{
    return (Vector2){std::stof(words[1]), std::stof(words[2])};
}

int read_face(const vector<string>& words)
{
    for (size_t i = 1; i < words.size(); i++)
    {
        cout << words[i] << endl;

        for (auto& s : string_split(words[i], "/"))
        {
            cout << s << endl;
        }
    }
    // TODO: implement
    return 1;
}

void load_obj_data(const string& path, vector<Vector3>& v, vector<Vector2>& vt, vector<Vector3>& vn, vector<int>& f)
{
    string line;
    vector<string> words;

    std::ifstream file;

    file.open(path);
    while (std::getline(file, line))
    {
        words = string_split(line, " ");

        if (!words[0].compare("v"))
        {
            v.push_back(read_vec3(words));
        }

        else if (!words[0].compare("vt"))
        {
            vt.push_back(read_vec2(words));
        }

        else if (!words[0].compare("vn"))
        {
            vn.push_back(read_vec3(words));
        }

        else if (!words[0].compare("f"))
        {
            cout << "reading faces" << endl;
            f.push_back(read_face(words));
        }
    }
    file.close();

    std::cout << "loaded some data" << std::endl;
    std::cout << "\tvertices: " << v.size() << std::endl;
    std::cout << "\ttex coords: " << vt.size() << std::endl;
    std::cout << "\tnormals: " << vn.size() << std::endl;
}

int main(void)
{
    vector<Vector3> v;
    vector<Vector2> vt;
    vector<Vector3> vn;
    vector<int> f;

    load_obj_data(get_full_path("res/cube.obj"), v, vt, vn, f);

    const int screenWidth = 400;
    const int screenHeight = 400;

    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, "Game");

    ssr::model_t cube = {

        .vertices =
            {
                (Vector3){1, 1, 1},    // 0
                (Vector3){-1, 1, 1},   // 1
                (Vector3){-1, -1, 1},  // 2
                (Vector3){1, -1, 1},   // 3
                (Vector3){1, 1, -1},   // 4
                (Vector3){-1, 1, -1},  // 5
                (Vector3){-1, -1, -1}, // 6
                (Vector3){1, -1, -1},  // 7
            },
        .triangles =
            {
                {.tri_indices = {0, 1, 2}, .color = RED},
                {.tri_indices = {0, 2, 3}, .color = RED},
                {.tri_indices = {4, 0, 3}, .color = GREEN},
                {.tri_indices = {4, 3, 7}, .color = GREEN},
                {.tri_indices = {5, 4, 7}, .color = BLUE},
                {.tri_indices = {5, 7, 6}, .color = BLUE},
                {.tri_indices = {1, 5, 6}, .color = GOLD},
                {.tri_indices = {1, 6, 2}, .color = GOLD},
                {.tri_indices = {4, 5, 1}, .color = PURPLE},
                {.tri_indices = {4, 1, 0}, .color = PURPLE},
                {.tri_indices = {2, 6, 7}, .color = BLACK},
                {.tri_indices = {2, 7, 3}, .color = BLACK},
            },
        .uv_of_each_vertex =
            {
                (Vector2){1, 0}, // 0
                (Vector2){0, 0}, // 1
                (Vector2){0, 1}, // 2
                (Vector2){0, 1}, // 3
                (Vector2){1, 1}, // 4
                (Vector2){0, 1}, // 5
                (Vector2){0, 0}, // 6
                (Vector2){1, 0}, // 7
            },
        .colors =
            {
                BLACK,
                BLACK,
                BLACK,
                BLACK,
                RED,
                GREEN,
                PURPLE,
                BLACK,
            }

    };

    ssr::instance_t cube_a = ssr::model_init_instance(cube,
                                                      (Vector3){0, 0, 7},
                                                      (Vector3){0, DEG2RAD * 0, 0},
                                                      (Vector3){1, 1, 1});
    ssr::instance_t cube_b = model_init_instance(cube,
                                                 (Vector3){0, 0, 3},
                                                 (Vector3){0, DEG2RAD * 30, 0},
                                                 (Vector3){1, 1, 1});

    ssr::camera_t camera = ssr::camera_t({0, 0, 0},
                                         (Vector3){0, DEG2RAD * 0, 0},
                                         90,
                                         0.1f,
                                         300.0f);

    ssr::Renderer renderer = ssr::Renderer();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        camera.move();
        camera.rotate();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        renderer.render_scene(vector<ssr::instance_t>{cube_b, cube_a}, camera);

        EndDrawing();
    }

    CloseWindow();
}