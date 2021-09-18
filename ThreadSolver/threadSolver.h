#ifndef THREADSOLVER_H
#define THREADSOLVER_H
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
//global mutex
extern pthread_mutex_t mutex;

struct args {
    struct matrix *matriz;
    int filaAct;
    int colAct;
    int dirAct;
};
int* elegirDireccion(struct matrix *self, int filaActual, int colActual, int dirActual);
void* realKeepGoing(void * currentStruct);
#endif