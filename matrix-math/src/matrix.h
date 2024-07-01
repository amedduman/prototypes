#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int rows;
  int cols;
  float* values;
} my_matrix;

void my_matrix_init(my_matrix* m, int rows, int cols)
{
  m->rows = rows;
  m->cols = cols;
  m->values = (float*)malloc(rows * cols * sizeof(float));

  for (int i = 0; i < rows * cols; i++)
  {
    m->values[i] = i;
  }
}

void my_matrix_print(my_matrix* m)
{
  for (int i = 0; i < m->rows * m->cols; i++)
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
  if(index < 0 || index >= m->rows * m->cols)
  {
    printf("there is a mistake at calculation of index. This shouldn't happen");
    return -2;
  }
  return m->values[index];
}