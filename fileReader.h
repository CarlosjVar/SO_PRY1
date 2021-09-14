#ifndef FILEREADER_H
#define FILEREADER .H
#include <stdio.h>
#include <stdlib.h> 

typedef struct fileReader fileReader;

struct fileReader
{
    FILE * fp;
    char linelen[255];
    char *matrix_ ="";
    void (*readFileLen)(fileReader * self);
};
fileReader* newFileReader();

#endif