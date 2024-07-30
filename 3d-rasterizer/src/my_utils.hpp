#pragma once

#pragma region print

/* template <typename T>
void print(const T& msg)
{
  using namespace std;
  cout << msg << endl;
} */

#pragma endregion

#pragma region vector-matrix operator overloading

// Operator overloading for Vector2 addition
/* Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2Add(v1, v2);
} */

/* Matrix operator *(const Matrix& m1, const Matrix& m2)
{
  return MatrixMultiply(m1, m2);
} */

// Operator overloading for Vector2 output
/* std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
} */

#pragma endregion

typedef struct 
{
  int x;
  int y;
} vec2i_t;

/* Vector2 to_vec2(Vector3 v)
{
  return (Vector2){v.x, v.y};
} */