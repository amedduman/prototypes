#pragma once

#include "../include/raylib.h"
#include "../include/raymath.h"
#include <iostream>
#include <vector>

namespace ssr
{

    class renderer
    {
    private:
    public:
    };

    struct vec2i_t
    {
        int x;
        int y;
    };

    class camera_t
    {
    public:
        Vector3 position = {};
        Vector3 rot_in_rad = {};
        float fov = {};
        float z_near = {};
        float z_far = {};
        float move_speed = {};
        float rot_speed = {};

        camera_t(Vector3 position, Vector3 rot_in_rad, float fov, float z_near, float z_far)
        {
            this->position = position;
            this->rot_in_rad = rot_in_rad;
            this->fov = fov;
            this->z_near = z_near;
            this->z_far = z_far;
        }

        void move()
        {
            Vector3 m = {0};

            if (IsKeyDown(KEY_A))
                m.x = -1;
            if (IsKeyDown(KEY_D))
                m.x = 1;
            if (IsKeyDown(KEY_W))
                m.y = 1;
            if (IsKeyDown(KEY_S))
                m.y = -1;

            m = Vector3Scale(m, move_speed);
            position = Vector3Add(position, m);
        }

        void rotate()
        {
            float s = rot_speed;
            if (IsKeyDown(KEY_LEFT))
                rot_in_rad = Vector3Add(rot_in_rad, (Vector3){DEG2RAD * 0, -DEG2RAD * s, DEG2RAD * 0});
            if (IsKeyDown(KEY_RIGHT))
                rot_in_rad = Vector3Add(rot_in_rad, (Vector3){DEG2RAD * 0, DEG2RAD * s, DEG2RAD * 0});
            if (IsKeyDown(KEY_UP))
                rot_in_rad = Vector3Add(rot_in_rad, (Vector3){-DEG2RAD * s, DEG2RAD * 0, DEG2RAD * 0});
            if (IsKeyDown(KEY_DOWN))
                rot_in_rad = Vector3Add(rot_in_rad, (Vector3){DEG2RAD * s, DEG2RAD * 0, DEG2RAD * 0});
        }
    };

#pragma region model
    struct triangle_t
    {
        int tri_indices[3];
        Color color;
    };

    struct model_t
    {
        std::vector<Vector3> vertices;
        std::vector<triangle_t> triangles;
        std::vector<Vector2> uv_of_each_vertex;
        std::vector<Color> colors;
    };

    struct transform_t
    {
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
    };

    struct instance_t
    {
        model_t model;
        transform_t transform;
    };

    instance_t model_init_instance(const model_t &model, Vector3 pos, Vector3 rot_in_rad, Vector3 my_scale)
    {
        instance_t instance = {
            .model = model,
            .transform = {
                .position = pos,
                .rotation = rot_in_rad,
                .scale = my_scale}};

        return instance;
    }
#pragma endregion

#pragma region transformations

    Matrix get_projection_matrix(const camera_t &cam)
    {
        float a = (float)GetScreenHeight() / (float)GetScreenWidth();
        ;
        float fov = cam.fov * DEG2RAD;
        float f = 1.0f / (tanf(fov / 2.0f));
        float z1 = cam.z_far / (cam.z_far - cam.z_near);
        float z2 = -(cam.z_far / (cam.z_far - cam.z_near)) * cam.z_near;

        Matrix proj = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0};

        proj.m0 = a * f;
        proj.m5 = f;
        proj.m10 = z1;
        proj.m14 = z2;
        proj.m11 = 1;

        return proj;
    }

    Vector4 mul_v3_mat(Vector3 v3, Matrix m)
    {
        // Convert Vector3 to Vector4
        Vector4 v = {v3.x, v3.y, v3.z, 1.0f};

        // Perform matrix multiplication (column-major order)
        Vector4 result;
        result.x = v.x * m.m0 + v.y * m.m4 + v.z * m.m8 + v.w * m.m12;
        result.y = v.x * m.m1 + v.y * m.m5 + v.z * m.m9 + v.w * m.m13;
        result.z = v.x * m.m2 + v.y * m.m6 + v.z * m.m10 + v.w * m.m14;
        result.w = v.x * m.m3 + v.y * m.m7 + v.z * m.m11 + v.w * m.m15;

        return result;
    }

    Vector3 transform_to_world_space(Vector3 v, const transform_t &tr)
    {
        Matrix s = MatrixScale(tr.scale.x, tr.scale.y, tr.scale.z);
        Matrix r = MatrixRotateZYX(tr.rotation);
        Matrix t = MatrixTranslate(tr.position.x, tr.position.y, tr.position.z);

        Matrix transform_matrix = MatrixMultiply(MatrixMultiply(s, r), t);

        return Vector3Transform(v, transform_matrix);
    }

    Vector3 transform_to_camera_space(Vector3 v_world, const camera_t &cam)
    {
        Vector3 v_translated = Vector3Subtract(v_world, cam.position);

        Vector3 v_camera = Vector3Transform(v_translated, MatrixInvert(MatrixRotateZYX(cam.rot_in_rad)));

        return v_camera;
    }

    Vector4 apply_projection_matrix(Vector3 v, camera_t cam)
    {
        return mul_v3_mat(v, get_projection_matrix(cam));
    }

    Vector3 apply_perspective_division(Vector4 v)
    {
        Vector3 result = {0};

        result.x = v.x / v.w;
        result.y = v.y / v.w;
        result.z = v.z / v.w;

        return result;
    }

    Vector2 map_ndc_to_screen(Vector3 v_ndc)
    {
        // Scale from [-1, 1] to [0, 1];
        float screen_x = (v_ndc.x + 1) / 2;
        float screen_y = (1 - v_ndc.y) / 2; // Flip Y-axis

        // Scale to screen dimensions
        screen_x *= GetScreenWidth();
        screen_y *= GetScreenHeight();

        return {screen_x, screen_y};
    }

#pragma endregion

#pragma region rendering
    // finds the cross product beween ab and ap vectors
    int edge_cross(vec2i_t a, vec2i_t b, vec2i_t p)
    {
        vec2i_t ab = {b.x - a.x, b.y - a.y};
        vec2i_t ap = {p.x - a.x, p.y - a.y};

        return ab.x * ap.y - ab.y * ap.x;
    }

    bool edge_is_top_or_left(vec2i_t start, vec2i_t end)
    {
        vec2i_t edge = {end.x - start.x, end.y - start.y};

        bool is_top = edge.y == 0 && edge.x > 0;
        bool is_left = edge.y < 0;

        return is_top || is_left;
    }

    Image crateTexture = LoadImage("/Users/amedduman/Documents/projects/raylib-C/3d-rasterizer/crate.png");
    Color *crateColors = LoadImageColors(crateTexture);

    // provided triangles need to be clock-wise
    void triangle_draw(const triangle_t &triangle, const std::vector<Vector3> camera_space_vertices, const std::vector<Vector2> &screen_vertices, std::vector<float> &inv_z_buffer, const instance_t &model)
    {
        vec2i_t v0 = {(int)screen_vertices[triangle.tri_indices[0]].x, (int)screen_vertices[triangle.tri_indices[0]].y};
        vec2i_t v1 = {(int)screen_vertices[triangle.tri_indices[1]].x, (int)screen_vertices[triangle.tri_indices[1]].y};
        vec2i_t v2 = {(int)screen_vertices[triangle.tri_indices[2]].x, (int)screen_vertices[triangle.tri_indices[2]].y};

        int x_min = std::min({v0.x, v1.x, v2.x});
        int y_min = std::min({v0.y, v1.y, v2.y});
        int x_max = std::max({v0.x, v1.x, v2.x});
        int y_max = std::max({v0.y, v1.y, v2.y});

        // find the area of triangle
        float area = edge_cross(v0, v1, v2);

        int bias0 = edge_is_top_or_left(v0, v1) ? 0 : -1;
        int bias1 = edge_is_top_or_left(v1, v2) ? 0 : -1;
        int bias2 = edge_is_top_or_left(v2, v0) ? 0 : -1;

        for (int x = x_min; x <= x_max; x++)
        {
            for (int y = y_min; y <= y_max; y++)
            {
                vec2i_t p = {x, y};

                // areas
                int w0 = edge_cross(v0, v1, p) + bias0;
                int w1 = edge_cross(v1, v2, p) + bias1;
                int w2 = edge_cross(v2, v0, p) + bias2;

                bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

                if (is_inside)
                {
                    // calculate barycentric weight of each vertex for the point
                    float v0_f = (float)w1 / area;
                    float v1_f = (float)w2 / area;
                    float v2_f = (float)w0 / area;

                    Color texelColor = MAGENTA;

                    // calculate depth for z depth test (in camera space)
                    float z0 = camera_space_vertices[triangle.tri_indices[0]].z;
                    float z1 = camera_space_vertices[triangle.tri_indices[1]].z;
                    float z2 = camera_space_vertices[triangle.tri_indices[2]].z;

                    // interpolated 1/z value (in camera space)
                    float depth = 1 / (z0 * v0_f + z1 * v1_f + z2 * v2_f);

                    // perspective-correct texture mapping
                    // by dividing z(z value comes from camera space btw) we are essentially applying perspective division
                    // to the uv coordinates. this way we count for perspective when we are doing our texture mapping.
                    float u0 = model.model.uv_of_each_vertex[triangle.tri_indices[0]].x / z0;
                    float v0 = model.model.uv_of_each_vertex[triangle.tri_indices[0]].y / z0;
                    float u1 = model.model.uv_of_each_vertex[triangle.tri_indices[1]].x / z1;
                    float v1 = model.model.uv_of_each_vertex[triangle.tri_indices[1]].y / z1;
                    float u2 = model.model.uv_of_each_vertex[triangle.tri_indices[2]].x / z2;
                    float v2 = model.model.uv_of_each_vertex[triangle.tri_indices[2]].y / z2;

                    // we are interpolate this perspective divided uv values by barycentric weights.
                    // this makes sense because when we apply perspective division to the uv coordinates
                    // we basically project them onto screen.
                    // so we can interpolate them by barycentric weights which also comes from rectangle that is porjected onto screen.
                    // we are interpolating in the same space.
                    // after we are done with interpolation we are reverse the perspective effect by dividing depth which itself is 1/z
                    float u = (u0 * v0_f + u1 * v1_f + u2 * v2_f) / depth;
                    float v = (v0 * v0_f + v1 * v1_f + v2 * v2_f) / depth;

                    u = Clamp(u, 0, 1);
                    v = Clamp(v, 0, 1);

                    int tex_x = (int)(u * (crateTexture.width - 1));
                    int tex_y = (int)(v * (crateTexture.height - 1));

                    int index = tex_y * crateTexture.width + tex_x;
                    texelColor = crateColors[index];

                    // check depth and draw pixel
                    if (depth > inv_z_buffer[y * GetScreenWidth() + x])
                    {
                        inv_z_buffer[y * GetScreenWidth() + x] = depth;
                        DrawPixel(x, y, texelColor);
                    }
                }
            }
        }
    }

    bool is_back_face(const triangle_t &triangle, const std::vector<Vector3> &cam_space_verts)
    {
        Vector3 triangle_position = Vector3Scale(Vector3Add(Vector3Add(
                                                                cam_space_verts[triangle.tri_indices[0]],
                                                                cam_space_verts[triangle.tri_indices[1]]),
                                                            cam_space_verts[triangle.tri_indices[2]]),
                                                 1.0f / 3.0f);

        // get point_to_cam vector
        Vector3 v = Vector3Negate(triangle_position); // in the cam space cam is in origin, no need to substract cam's position to get the vector.

        // clock wise order
        Vector3 a = cam_space_verts[triangle.tri_indices[0]];
        Vector3 b = cam_space_verts[triangle.tri_indices[1]];
        Vector3 c = cam_space_verts[triangle.tri_indices[2]];

        // get normal
        Vector3 ab = Vector3Subtract(b, a);
        Vector3 ac = Vector3Subtract(c, a);
        Vector3 n = Vector3CrossProduct(ab, ac);

        float bf = Vector3DotProduct(v, n);

        // if dot product is negetive between normal of the surface and vector to camera then don't draw it because it is a back-face triangle
        if (bf <= 0)
        {
            return true;
        }
        return false;
    }

    void render_model_instance(const instance_t &instance, const camera_t &cam, std::vector<float> &inv_z_buffer)
    {
        using namespace std;
        vector<Vector2> protected_vertices = {};
        vector<Vector3> camera_space_vertices = {};
        for (size_t i = 0; i < instance.model.vertices.size(); i++)
        {
            Vector3 v_world = transform_to_world_space(instance.model.vertices[i], instance.transform);

            Vector3 v_camera = transform_to_camera_space(v_world, cam);
            camera_space_vertices.push_back(v_camera);

            Vector4 v_proj_applied = apply_projection_matrix(v_camera, cam);

            Vector3 v_perspective_applied = apply_perspective_division(v_proj_applied);

            Vector2 v_screen = map_ndc_to_screen(v_perspective_applied);

            protected_vertices.push_back(v_screen);
        }

        for (size_t i = 0; i < instance.model.triangles.size(); i++)
        {
            if (is_back_face(instance.model.triangles[i], camera_space_vertices))
                continue;

            triangle_draw(instance.model.triangles[i], camera_space_vertices, protected_vertices, inv_z_buffer, instance);
        }
    }

    void render_scene(std::vector<instance_t> scene, camera_t cam)
    {
        using namespace std;
        vector<float> inv_z_buffer;
        int size = GetScreenWidth() * GetScreenHeight();
        for (int i = 0; i < size; i++)
        {
            inv_z_buffer.push_back(0);
        }

        for (size_t i = 0; i < scene.size(); i++)
        {
            render_model_instance(scene[i], cam, inv_z_buffer);
        }
    }

#pragma endregion

}
