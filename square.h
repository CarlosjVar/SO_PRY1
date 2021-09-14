#ifndef SQUARE_H
#define SQUARE .H
#include <stdbool.h>

typedef struct square square;

struct square
{
    int x, y;
    char type;
    bool up, down, left , right ;

};
square* newSquare(int i, int j, char t_);

#endif