
#include <stdio.h>
#include <stdlib.h>
#include "Matrix/matrix.h"
#include "FileReader/fileReader.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include "ForkSolver/ForkSolver.h"

#define ANSI_COLOR_WALL "\x1B[38;2;37;92;87m"
#define ANSI_COLOR_PATH "\x1B[38;2;247;197;146m"
#define ANSI_COLOR_PATH_2 "\x1B[38;2;88;24;69m"
#define ANSI_COLOR_PATH_3 "\x1B[38;2;88;24;69m"
#define ANSI_COLOR_PATH_4 "\x1B[38;2;88;24;69m"
#define ANSI_COLOR_GOAL "\x1B[38;2;97;164;124m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void printMatrix(matrix *self)
{
    for (int i = 0; i < self->rows; i = i + 1)
    {
        for (int j = 0; j < self->cols; j = j + 1)
        {
            square actual = self->matrix_[i][j];
            if (actual.type=='*')
                printf(ANSI_COLOR_WALL "██");
            if (actual.type=='/')
                printf(ANSI_COLOR_GOAL "▓▓");
            if (actual.type==' '){
                switch (actual.times)
                {
                case 1:
                    printf(ANSI_COLOR_PATH_2 "░░");
                    break;
                case 2:
                    printf(ANSI_COLOR_PATH_3 "░░");
                    break;
                case 3:
                    printf(ANSI_COLOR_PATH_4 "░░");
                    break;
                default:
                    printf(ANSI_COLOR_PATH "░░");
                    break;
                }

            }
                
            
        }
        printf("%c", '\n');
    }
    
}

char *append(const char *s, char c)
{
    int len = strlen(s);
    char buf[len + 2];
    strcpy(buf, s);
    buf[len] = c;
    buf[len + 1] = 0;
    return strdup(buf);
}

void getMatrixSize(char *str, matrix *matrix_)
{
    char *ch;
    int cont = 0;
    char numr[10] = "";
    char numc[10] = "";

    int i = 0;
    while (str[i] != ' ')
    {
        numr[cont] = str[i];
        i++;
        cont++;
    }
    cont = 0;
    while (i < strlen(str))
    {
        numc[cont] = str[i];
        i++;
        cont++;
    }

    matrix_->rows = atoi(numr);
    matrix_->cols = atoi(numc);
    return;
}

square **createMatrix(matrix *self)
{
    square **matrix = (square **)malloc(self->rows * sizeof(square *));
    for (int i = 0; i < self->rows; i = i + 1)
    {
        matrix[i] = (square *)malloc(self->cols * sizeof(square));
    }
    int cont = 0;
    for (int i = 0; i < self->rows; i = i + 1)
    {
        for (int j = 0; j < self->cols; j = j + 1)
        {
            matrix[i][j] = *newSquare(i, j, self->path[cont]);
            cont++;
        }
    }
    self->matrix_ = matrix;
    return matrix;
}

square **createMatrixFork(struct matrix*self){
    square **matrix = (square **)mmap( NULL, self->rows *sizeof(square),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, 0 ,0);
    for (int i = 0; i < self->rows; i = i + 1)
    {
        matrix[i] = (square *)mmap( NULL, self->cols *sizeof(square),
         PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, 0, 0 );
    }
    int cont = 0;
    for (int i = 0; i < self->rows; i = i + 1)
    {
        for (int j = 0; j < self->cols; j = j + 1)
        {
            matrix[i][j] = *newSquare(i, j, self->path[cont]);
            cont++;
        }
    }
    self->matrix_ = matrix;
    return matrix;
}



void readFileLen(fileReader *self)
{
    int i = 0;
    int ch;
    int bufferLength = 255;
    char buffer[bufferLength];
    char *boxes = "";

    self->fp = fopen("./Laberintos/lab1.txt", "r");
    if (self->fp == NULL)
        exit(EXIT_FAILURE);

    fgets(buffer, bufferLength, self->fp);
    strcpy(self->linelen, buffer);

    while ((ch = fgetc(self->fp)) != EOF)
    {
        if (ch != '\r' && ch != '\n')
        {
            boxes = append(boxes, ch);
        }
    }
    self->matrix_ = boxes;
    fclose(self->fp);
}

int main(int argc, char *argv[])
{
    fileReader *reader = newFileReader();
    reader->readFileLen(reader);
    
    struct matrix *realMatrix = newMatrix();
    struct matrix *realMatrix2 = newMatrix();

    realMatrix->getMatrixSize(reader->linelen, realMatrix);
    realMatrix->path = reader->matrix_;
    realMatrix2 = realMatrix;

    realMatrix->createMatrix(realMatrix);
    realMatrix2->createMatrixFork(realMatrix2);
    realMatrix->printMatrix(realMatrix);

    chooseDirection(realMatrix2,0,0);

    exit(EXIT_SUCCESS);
    return 0;
}

fileReader *newFileReader()
{
    fileReader *self = (fileReader *)malloc(sizeof(fileReader));
    self->readFileLen = readFileLen;
    return self;
}

matrix *newMatrix()
{
    matrix *self = (matrix *)malloc(sizeof(matrix));
    self->getMatrixSize = getMatrixSize;
    self->createMatrix = createMatrix;
    self->createMatrixFork = createMatrixFork;
    self->printMatrix = printMatrix;
    self->finished = (bool*)mmap(NULL,1*sizeof(bool),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, 0 ,0);
    return self;
}

square *newSquare(int x_, int y_, char t_)
{
    square *self = (square *)malloc(sizeof(square));
    self->down = false; self->up = false; self->left = false; self->right = false;
    self->x = x_; self->y = y_; self->type = t_; self->times = 0;

    return self;
}

void* chooseDirection( matrix *matrix, int filaActual, int colActual){
    printf("Hola \n");
    //Llegó al objetivo
    if(matrix->matrix_[filaActual][colActual].type =='/'){
            matrix->finished = true ;
        }

    // Moverse abajo
    if(filaActual-1 >= 0){
        if(matrix->matrix_[filaActual-1][colActual].type!= '*'){

        printf("Voy para abajo \n");
        createForkChilds(matrix,filaActual,colActual,0);
        }
    }
    // Moverse arriba
    if(filaActual < matrix->rows-1){
        if(matrix->matrix_[filaActual][colActual].type!= '*'){
        
        printf("Voy para arriba \n");
        createForkChilds(matrix,filaActual,colActual,1);
        }
    }
    //Moverse izquierda
    if(colActual-1 >= 0){
        if(matrix->matrix_[filaActual][colActual-1].type!= '*'){
        printf("Voy para la izquierda \n");
        createForkChilds(matrix,filaActual,colActual,2);
        }
    }
    //Moverse derecha
    if(colActual < matrix->cols-1){
        if(matrix->matrix_[filaActual][colActual+1].type!= '*'){
        printf("Voy para la derecha \n");
        createForkChilds(matrix,filaActual,colActual,3);
        }
    }
    else
    {
        printf("No encontré ni monda");
    }
    wait(NULL);
    return ;
}

void* travelMatrix(struct matrix*matrix, int filaActual, int colActual, int direccion){

    while(filaActual >= 0 && colActual >= 0 && filaActual < matrix->rows-1 && colActual < matrix->cols-1){

        if(direccion == 0){
            printf("El caracter %c  \n",matrix->matrix_[filaActual-1][colActual].type);
            if(matrix->matrix_[filaActual-1][colActual].type == "*")
            {
                printf("Chocó");
                break;
            }
            filaActual--;
            matrix->matrix_[filaActual][colActual].down==true;
        }
        else if(direccion == 1){
            printf("El caracter %c  \n",matrix->matrix_[filaActual+1][colActual].type);
            if(matrix->matrix_[filaActual+1][colActual].type == '*')
            {
                printf("Chocó");
                break;
            }
            filaActual++;
            matrix->matrix_[filaActual][colActual].up==true;
        }
        else if(direccion == 2){
            printf("El caracter %c  \n",matrix->matrix_[filaActual][colActual-1].type);
            if(matrix->matrix_[filaActual][colActual-1].type == "*")
            {   printf("Chocó");
                break;
            }
            colActual--;
            matrix->matrix_[filaActual][colActual].left==true;
        }
        else if (direccion == 3){
            printf("El caracter %c  \n",matrix->matrix_[filaActual][colActual+1].type);
            if(matrix->matrix_[filaActual+1][colActual].type == "*")
            {
                printf("Chocó");
                break;
            }
            colActual++;
            matrix->matrix_[filaActual][colActual].right==true;
        }
       
      
    }
    printf("Terminó de recorrer este hijo \n \n");
    // Se deben crear forks para continuar ya que se topó con una pared
    chooseDirection(matrix,filaActual,colActual);
}
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual,int direction){

        pid_t pid= fork();
        if(pid==0)
        {
            printf("Se creó un hijo \n");
            travelMatrix(matrix,filaActual,colActual,direction);
        }



}