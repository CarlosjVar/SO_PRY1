#ifndef FORKSOLVER_H
#define FORKSOLVER_H
#include <stdio.h>
#include <stdlib.h>
#include "../Matrix/matrix.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include<sys/wait.h>
#include <unistd.h>
#include <pthread.h>
int* selectDirection(struct matrix *self, int filaActual, int colActual, int dirActual);
void* travelMatrix(struct matrix*matrix, int filaActual, int colActual, int direction);
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual, int direction);

#endif