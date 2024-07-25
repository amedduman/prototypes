#include "include/raylib.h"
#include "include/raymath.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

// #region Lights

typedef enum
{
  e_light_type_ambient = 0,
  e_light_type_point = 1,
  e_light_type_directional = 2
} light_type;

typedef struct
{
  float intensity;
} light_ambient;

typedef struct
{
  float intensity;
  Vector3 position;
} light_point;

typedef struct
{
  float intensity;
  Vector3 direction;
} light_directional;

typedef struct
{
  light_type my_type;
  union 
  {
    light_ambient ambient;
    light_point point;
    light_directional directional;
  };
} light;

// #endregion

// #region LightCalculations

float compute_light(const light* lights, int light_count, Vector3 point, Vector3 normal)
{
  assert(Vector3Length(normal) < 1 + EPSILON);
  float intensity = 0.0f; // intensity
  Vector3 L = {0,0,0}; // coming light vector
  for (int i = 0; i < light_count; i++)
  {
    light current_light = lights[i];

    if (current_light.my_type == e_light_type_ambient)
    {
      intensity += current_light.ambient.intensity;
    }
    else
    {
      float light_intensity = 0;

      if (current_light.my_type == e_light_type_point)
      {
        L = Vector3Subtract(current_light.point.position, point);
        light_intensity = current_light.point.intensity;
      }
      else if (lights[i].my_type == e_light_type_directional)
      {
        L = current_light.directional.direction;
        light_intensity = current_light.directional.intensity;
      }

      float n_dot_l = Vector3DotProduct(normal, L);
      if (n_dot_l > 0)
      {
        intensity += light_intensity * n_dot_l / (Vector3Length(normal) * Vector3Length(L));
      }
    }
  }
  return intensity;
}
// #endregion

// #region Sphere
typedef struct
{
  Vector3 center;
  float radius;
  Color color;
} Sphere;
// #endregion

// #region Canvas

  void canvas_put_pixel(int Cx, int Cy, Color color)
  {
    int Sx = GetScreenWidth() / 2 + Cx;
    int Sy = GetScreenHeight() / 2 - Cy;

    DrawPixel(Sx, Sy, color);
  }

 
  Vector3 canvas_to_viewport(int Cx, int Cy)
  {
    float Vw = 1.0f; // the viewport height
    float Vh = 1.0f; // the viewport width
    float d = 1.0f; // the distance from the camera to the canvas

    float Vx = Cx * (Vw / GetScreenWidth());
    float Vy = Cy * (Vh / GetScreenHeight());
    float Vz = d;

    return (Vector3){ Vx, Vy, Vz };
  }

// #endregion

// #region Raycasting

  typedef struct
  {
    bool is_intersecting;
    Vector2 intersections;
  } ray_sphere_intersection_info;

  // https://www.youtube.com/watch?v=OCZTVpfMSys
  ray_sphere_intersection_info ray_sphere_intersection(Vector3 so, float sr, Vector3 ro, Vector3 rd)
  {
    Vector3 CO = Vector3Subtract(ro, so);

    float a = Vector3DotProduct(rd, rd);
    float b = 2 * Vector3DotProduct(CO, rd);
    float c = Vector3DotProduct(CO, CO) - (sr * sr);

    float discriminant = (b * b) - (4 * a * c); 

    if (discriminant < 0)
    {
      return (ray_sphere_intersection_info) {false, (Vector2){-1,-1}};
    }

    float t1 = (-b + sqrt(discriminant)) / (2*a);
    float t2 = (-b - sqrt(discriminant)) / (2*a);

    return (ray_sphere_intersection_info) {true, (Vector2){t1,t2}};
  }
// #endregion

// #region Raytracing
Color trace_ray(Sphere* spheres, int sphere_count, light* lights, int light_count, Vector3 ro, Vector3 rd, float tmin, float tmax)
{
  float closest_t = INFINITY;
  bool haveFoundSphereToDraw = false;
  Sphere closestSphere;

  for (int i = 0; i < sphere_count; i++)
  {
    ray_sphere_intersection_info hit_info = ray_sphere_intersection(spheres[i].center, spheres[i].radius, ro, rd);

    float t1 = hit_info.intersections.x;
    float t2 = hit_info.intersections.y;

    if (hit_info.is_intersecting)
    { 
      if (t1 > tmin && t1 < tmax)
      {
        if (t1 < closest_t)
        {
          haveFoundSphereToDraw = true;
          closest_t = t1;
          closestSphere = spheres[i];
        }
      }
      
      if (t2 > tmin && t2 < tmax)
      {
        if (t2 < closest_t)
        {
          haveFoundSphereToDraw = true;
          closest_t = t2;
          closestSphere = spheres[i];
        }
      }
    }
  }

  if (haveFoundSphereToDraw)
  {
    Vector3 intersection_point = Vector3Add(ro, Vector3Scale(rd, closest_t));
    Vector3 surface_normal = Vector3Subtract(intersection_point, closestSphere.center);
    surface_normal = Vector3Normalize(surface_normal);
    
    float light_intensity = compute_light(lights, light_count, intersection_point, surface_normal);
    Color resulted_color = {
      (unsigned char)(closestSphere.color.r * light_intensity),
      (unsigned char)(closestSphere.color.g * light_intensity),
      (unsigned char)(closestSphere.color.b * light_intensity),
      255
    };

    return resulted_color;
  }
  else
  {
    return GOLD;
  }
}
// #endregion

// #region Main

#define sphere_count 3
#define light_count 3

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Game");
    
    Sphere spheres[sphere_count] = {
      {.center = {0, -1, 3}, .radius = 1, RED},
      {.center = {2, 0, 4}, .radius = 1, GREEN},
      {.center = {-2, 0, 4}, .radius = 1, BLUE},
    };

    light lights[light_count] = {
      {e_light_type_ambient, .ambient = {0.2f}},
      {e_light_type_point, .point = {0.6f, {2, 1, 0}}},
      {e_light_type_directional, .directional = {0.2f, {1, 4, 4}}}
    };



    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      BeginDrawing();

      ClearBackground(RAYWHITE);

      Vector3 ro = {0,0,0};
      int w = GetScreenWidth();
      int h = GetScreenWidth();
      for (int x = -w/2; x < w/2; x++)
      {
        for (int y = -h/2; y < h/2; y++)
        {
          Vector3 rd = canvas_to_viewport(x, y);
          Color color = trace_ray(spheres, sphere_count, lights, light_count, ro, rd, 1, INFINITY);
          canvas_put_pixel(x, y, color);
        }
      }

      EndDrawing();
    }
    CloseWindow();
    return 0;
}

// #endregion