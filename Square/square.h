#ifndef SQUARE_H
#define SQUARE .H
#include <stdbool.h>

typedef struct square square;

struct square
{
    int x; 
    int y;
    char type;
    bool up, down, left , right ;
    int times;
};
square* newSquare(int i, int j, char t_);

#endif