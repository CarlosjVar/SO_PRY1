#ifndef FORKSOLVER_H
#define FORKSOLVER_H
#include <stdio.h>
#include <stdlib.h>
#include "../Matrix/matrix.h"
void* chooseDirection(struct matrix*matrix , int filaActual, int colActual,int direction);
void* travelMatrix(struct matrix*matrix, int filaActual, int colActual, int direction);
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual, int direction);
#endif