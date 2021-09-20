
#include <stdio.h>
#include "Matrix/matrix.h"
#include "FileReader/fileReader.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include<sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "ForkSolver/ForkSolver.h"
#include "ThreadSolver/threadSolver.h"
#include <stdlib.h>

#define ANSI_COLOR_WALL "\x1B[38;2;37;92;87m"
#define ANSI_COLOR_PATH "\x1B[38;2;247;197;146m"
#define ANSI_COLOR_PATH_2 "\x1B[38;2;255;87;51m"
#define ANSI_COLOR_PATH_3 "\x1B[38;2;199;0;57m"
#define ANSI_COLOR_PATH_4 "\x1B[38;2;144;12;63m"
#define ANSI_COLOR_GOAL "\x1B[38;2;97;164;124m"
#define ANSI_COLOR_RESET   "\x1b[0m"
pthread_mutex_t mutex;

void printMatrix(matrix *self)
{
    //system("clear");
    for (int i = 0; i < self->rows; i = i + 1)
    {
        for (int j = 0; j < self->cols; j = j + 1)
        {
            pthread_mutex_lock(&self->lock);
            square actual = self->matrix_[i][j];
            pthread_mutex_unlock(&self->lock);
            if (actual.type=='*')
                printf(ANSI_COLOR_WALL "██");
            if (actual.type=='/')
            {
                switch(actual.times)
                {
                    case 1:
                        printf(ANSI_COLOR_RESET "▓▓");
                        break;
                    default:
                        printf(ANSI_COLOR_GOAL "▓▓");
                        break;

                }

            }

            if (actual.type==' '){
                switch (actual.times)
                {
                case 1:
                    printf(ANSI_COLOR_PATH_2 "▓▓");
                    break;
                case 2:
                    printf(ANSI_COLOR_PATH_3 "▓▓");
                    break;
                case 0:
                    printf(ANSI_COLOR_PATH "░░");
                    break;
                default:
                    printf(ANSI_COLOR_PATH_4 "▓▓");
                    break;
                }

            }
                
            
        }
        printf("%c", '\n');
    }
}

void *Paint(void *self)
{

    while(true)
    {
        sleep(3);
        struct matrix *m = (matrix*)self;
        printMatrix(m);
    }

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

square **createMatrixFork(matrix*self){
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

    self->fp = fopen("./Laberintos/lab2.txt", "r");
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
    // File Reader setup
    fileReader *reader = newFileReader();
    reader->readFileLen(reader);
   
    // Matrix Struct creation
    
    struct matrix *realMatrix = newMatrix();
    struct matrix *realMatrix2 = newMatrixFork();
    
    // Thread Matrix setup
    realMatrix->getMatrixSize(reader->linelen, realMatrix);
    realMatrix->path = reader->matrix_;
    
    realMatrix->createMatrix(realMatrix);
    
    // Fork Matrix setup
    
    realMatrix2->rows = realMatrix->rows;
    realMatrix2->cols = realMatrix->cols;
    realMatrix2->path  = reader->matrix_;


    realMatrix2->createMatrixFork(realMatrix2);

    
    realMatrix2->lock = mutex;

    // Mutex setup
    pthread_mutex_init(&mutex,NULL);
    realMatrix2->lock = mutex;
    realMatrix->lock = mutex;
    pthread_t thread_id;
    
    // Start Threads 

    struct args *mainStruct = (struct args *)malloc(sizeof(struct args));
    mainStruct->matriz = realMatrix;
    mainStruct->filaAct = 0;
    mainStruct->colAct = 0;
    mainStruct->dirAct = 5;
    mainStruct->camRecorrido = 0;

    pthread_t mainthread;
    pthread_create(&mainthread,NULL,realKeepGoing, (void *)mainStruct);
    pthread_join(mainthread, NULL);

    realMatrix2->printMatrix(realMatrix2);

    // End threads
    //pthread_create(&thread_id, NULL, Paint, (void*) (&realMatrix2));
    //pthread_join(thread_id, NULL);

    //Fork start
    pid_t child_pid, wpid;
    int status = 0;
    time_t inicio = time(NULL);
    if (child_pid=fork()==0)
    {
        travelMatrix(realMatrix2,0,0,5,0);
        while ((wpid = wait(&status)) > 0); 
        _exit(0);
    }
    else{
        printf("Empezó de esperar \n");
        while ((wpid = wait(&status)) > 0);
        time_t final = time(NULL);
        printf("Duró %d segundos\n", final-inicio);
        realMatrix2->printMatrix(realMatrix2);
        sleep(1);
    }   
    //End Fork section
   

    printf("\n Final");


    pthread_mutex_destroy(&mutex);
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
    self->finished = (int*)mmap(NULL,1*sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, 0 ,0);
    self->finished = 0;
    return self;
}
matrix *newMatrixFork()
{
    matrix *self = (matrix *)mmap( NULL,sizeof(matrix),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, 0 ,0);
    self->getMatrixSize = getMatrixSize;
    self->createMatrix = createMatrix;
    self->createMatrixFork = createMatrixFork;
    self->printMatrix = printMatrix;
    self->finished = (int*)mmap(NULL,1*sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, 0 ,0);
    self->finished = 0;
    return self;
}

square *newSquare(int x_, int y_, char t_)
{
    square *self = (square *)malloc(sizeof(square));
    self->down = false; self->up = false; self->left = false; self->right = false;
    self->x = x_; self->y = y_; self->type = t_; self->times = 0;

    return self;
}