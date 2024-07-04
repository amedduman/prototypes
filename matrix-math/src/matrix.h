#pragma once
#include "../include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int rows;
  int cols;
  int elementCount;
  float* values;
} my_matrix;

void my_matrix_init(my_matrix* m, int rows, int cols)
{
  m->rows = rows;
  m->cols = cols;
  m->elementCount = rows * cols;
  m->values = (float*)malloc(m->elementCount * sizeof(float));

  for (int i = 0; i < m->elementCount; i++)
  {
    m->values[i] = 0;
  }
}

void my_matrix_randomize(my_matrix* m)
{
  for (int i = 0; i < m->elementCount; i++)
  {
    m->values[i] = rand() % 10;
  }
}

void my_matrix_print(my_matrix* m)
{
  for (int i = 0; i < m->elementCount; i++)
  {
    printf("%f\n",m->values[i]);
  }
}

float my_matrix_get_val(my_matrix* m, int row, int col)
{
  if(row < 1 || row > m->rows || col < 1 || col > m->cols)
  {
    printf("the given row and column values are not propreaite for the matrix");
    return -1;
  }
  row--;
  col--;
  int index = m->cols * row + col;
  if(index < 0 || index >= m->elementCount)
  {
    printf("there is a mistake at calculation of index. This shouldn't happen");
    return -2;
  }
  return m->values[index];
}

float my_matrix_get_val_zero_based(my_matrix* m, int row, int col)
{
  int index = m->cols * row + col;
  if(index < 0 || index >= m->elementCount)
  {
    printf("there is a mistake at calculation of index. This shouldn't happen");
    return -2;
  }
  return m->values[index];
}

void my_matrix_mul_with_scaler(my_matrix* m, float scaler)
{
  for (int i = 0; i < m->elementCount; i++)
  {
    m->values[i] *= scaler;
  }
}

void my_matrix_add_with_scaler(my_matrix* m, float scaler)
{
  for (int i = 0; i < m->elementCount; i++)
  {
    m->values[i] += scaler;
  }
}

typedef enum
{
  mul = 0,
  add = 1,
}element_wise_matrix_calculation_type;

static my_matrix my_matrix_element_wise_calculations(my_matrix* m1, my_matrix* m2, element_wise_matrix_calculation_type calc_type)
{
  my_matrix result_matrix;
  my_matrix_init(&result_matrix, m1->rows, m1->cols);

  if (m1->rows == m2->rows && m1->cols == m2->cols)
  {
    for (int i = 0; i < m1->elementCount; i++)
    {
      switch (calc_type)
      {
        case mul:
        result_matrix.values[i] = m1->values[i] * m2->values[i];
        break;

        case add:
        result_matrix.values[i] = m1->values[i] + m2->values[i];
        break;
      }
    }
  }
  else
  {
    printf("the rows and columns of matrixes are not matching");
  }

  return result_matrix;
}

my_matrix my_matrix_mul_with_matrix(my_matrix* m1, my_matrix* m2)
{
  return my_matrix_element_wise_calculations(m1, m2, mul);
}

my_matrix my_matrix_add_with_matrix(my_matrix* m1, my_matrix* m2)
{
  return my_matrix_element_wise_calculations(m1, m2, add);
}

static float mul_row_col_values(my_matrix* m1, int row, my_matrix* m2, int col)
{
  float total = 0;
  for (int i = 0; i < m1->cols; i++)
  {
    total += my_matrix_get_val_zero_based(m1, row, i) * my_matrix_get_val_zero_based(m2, i, col);
  }

  return total;
}

my_matrix my_matrix_marix_product(my_matrix* m1, my_matrix* m2)
{
  my_matrix result_matrix;
  my_matrix_init(&result_matrix, m1->rows, m2->cols);

  if(m1->cols != m2->rows)
  {
    printf("columns and rows count of two matrixes are not the same.\n");
    return result_matrix;
  }

  int result_matrix_index = 0;

  for (int i = 0; i < m1->rows; i++)
  {
    for (int j = 0; j < m2->cols; j++)
    {
      float result = mul_row_col_values(m1, i, m2, j);
      result_matrix.values[result_matrix_index] = result; // result_matrix.values[i * m1->rows + j] = result;
      result_matrix_index++;
    }
  }

  return result_matrix;
}

Vector2 my_matrix_mul_with_Vector2(my_matrix* m, Vector2 v)
{
  my_matrix vm;
  my_matrix_init(&vm, 3, 1);
  float vmValues[] = {v.x, v.y, 1};
  vm.values = vmValues;
  my_matrix result_matrix = my_matrix_marix_product(m, &vm);
  printf("------------\n");
  my_matrix_print(&vm);
  return (Vector2){result_matrix.values[0], result_matrix.values[1]};
}

void my_matrix_free(my_matrix *m)
{
    free(m->values);
}