
#include <stdio.h>
#include <stdlib.h>
#include "Matrix/matrix.h"
#include "FileReader/fileReader.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

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


void *Paint(void *self)
{
    sleep(3);
    struct matrix *m = (matrix*)self;
    printMatrix(m);
    return NULL;
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
    PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
    for (int i = 0; i < self->rows; i = i + 1)
    {
        matrix[i] = (square *)mmap( NULL, self->cols *sizeof(square),
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
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
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, Paint, (void*) (realMatrix2));
    pthread_join(thread_id, NULL);

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
    return self;
}

square *newSquare(int x_, int y_, char t_)
{
    square *self = (square *)malloc(sizeof(square));
    self->down = false; self->up = false; self->left = false; self->right = false;
    self->x = x_; self->y = y_; self->type = t_; self->times = 0;

    return self;
}