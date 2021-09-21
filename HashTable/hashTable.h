#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 1600

struct DataItem {
   int exito;
   int espacios;   
   int key;
};



void *display();
void *displayPid();
void *insert(int key,int data,int exito) ;
#endif