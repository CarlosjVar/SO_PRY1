#ifndef FORKSOLVER_H
#define FORKSOLVER_
#include <stdio.h>
#include <stdlib.h>
#include "../Matrix/matrix.h"
void* chooseDirection(struct matrix*matrix , int filaActual, int colActual);
void* travelMatrix(struct matrix*matrix, int filaActual, int colActual, int direccion);
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual, int direction);
#endif