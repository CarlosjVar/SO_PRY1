#ifndef MATRIX_H
#define MATRIX_H
#include <stdio.h>
#include <stdlib.h>
#include "../Square/square.h"

typedef struct matrix matrix;

struct matrix
{
    int rows, cols;
    char *path;
    square** matrix_;
    square** (*createMatrix)(matrix *self);
    square** (*createMatrixFork)(matrix *self);
    void (*printMatrix)(matrix *self);
    void (*getMatrixSize)(char *str, matrix *self);
};

matrix* newMatrix();

#endif