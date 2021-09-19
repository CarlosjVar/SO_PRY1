#ifndef THREADSOLVER_H
#define THREADSOLVER_H
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Matrix/matrix.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include<sys/wait.h>
#include <unistd.h>
//global mutex

struct args {
    struct matrix *matriz;
    int filaAct;
    int colAct;
    int dirAct;
    int camRecorrido;
};
int* elegirDireccion(struct matrix *self, int filaActual, int colActual, int dirActual);
void* realKeepGoing(void * currentStruct);
#endif