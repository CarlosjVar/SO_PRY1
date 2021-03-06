
#include <stdio.h>
#include "Matrix/matrix.h"
#include "FileReader/fileReader.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
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
#define ANSI_COLOR_RESET "\x1b[0m"
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
            if (actual.type == '*')
                printf(ANSI_COLOR_WALL "██");
            if (actual.type == '/')
            {
                switch (actual.times)
                {
                case 1:
                    printf(ANSI_COLOR_RESET "▓▓");
                    break;
                default:
                    printf(ANSI_COLOR_GOAL "▓▓");
                    break;
                }
            }

            if (actual.type == ' ')
            {
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

    while (true)
    {
        sleep(1);
        printMatrix((struct matrix *)self);
        if (((struct matrix *)self)->finished)
        {
            break;
        }
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

square **createMatrixFork(matrix *self)
{
    square **matrix = (square **)mmap(NULL, self->rows * sizeof(square),
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    for (int i = 0; i < self->rows; i = i + 1)
    {
        matrix[i] = (square *)mmap(NULL, self->cols * sizeof(square),
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED | MAP_ANONYMOUS, 0, 0);
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

    self->fp = fopen(self->path, "r");
    if (self->fp == NULL)
    {
        exit(EXIT_FAILURE);
    }

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
    printf("Archivo cargado. \n");
}

void *startForkSolution(matrix *matrix)
{
    time_t inicio = time(NULL);
    pid_t child_pid;
    if (child_pid = fork() == 0)
    {
        pid_t wpid2;
        int status2 = 0;
        travelMatrix(matrix, 0, 0, 5, 0);
        while ((wpid2 = wait(&status2)) > 0)
            ;
        _exit(0);
    }
    else
    {
        pid_t wpid;
        int status = 0;
        printf("Empezó de esperar \n");
        while ((wpid = wait(&status)) > 0)
            ;
        time_t final = time(NULL);
        printf("Duró %d segundos\n", final - inicio);
        sleep(1);
        matrix->finished = true;
    }
}

void *startThreadSolution(matrix *matrix)
{
    time_t inicio = time(NULL);
    struct args *mainStruct = (struct args *)malloc(sizeof(struct args));
    mainStruct->matriz = matrix;
    mainStruct->filaAct = 0;
    mainStruct->colAct = 0;
    mainStruct->dirAct = 5;
    mainStruct->camRecorrido = 0;

    pthread_t mainthread;
    pthread_create(&mainthread, NULL, realKeepGoing, (void *)mainStruct);
    pthread_join(mainthread, NULL);
    time_t final = time(NULL);
    printf("Duró %d segundos\n", final - inicio);
    sleep(1);
    matrix->finished = true;
}

int main(int argc, char *argv[])
{
    int n, opcion;
    fileReader *reader = newFileReader();
    struct matrix *realMatrix = newMatrix();
    struct matrix *realMatrix2 = newMatrixFork();
    do
    {
        printf("\n");
        printf("Tarea Programada #1 --- Menú \n");
        printf("1. Cargar matriz \n");
        printf("2. Correr hilos \n");
        printf("3. Correr forks \n");
        printf("4. Salir \n");
        printf("Ingrese una opción: \n");
        scanf("%d", &opcion);
        printf("\n");
        fileReader *reader = newFileReader();

        switch (opcion)
        {
        case 1:
        {
            printf("Ingrese el path: \n");
            scanf("%s", reader->path);
            reader->readFileLen(reader);

            // Thread Matrix setup
            realMatrix->getMatrixSize(reader->linelen, realMatrix);
            realMatrix->path = reader->matrix_;

            realMatrix->createMatrix(realMatrix);

            // Fork Matrix setup

            realMatrix2->rows = realMatrix->rows;
            realMatrix2->cols = realMatrix->cols;
            realMatrix2->path = reader->matrix_;

            realMatrix2->createMatrixFork(realMatrix2);

            realMatrix2->lock = mutex;

            // Mutex setup
            pthread_mutex_init(&mutex, NULL);
            realMatrix2->lock = mutex;
            realMatrix->lock = mutex;
        }
        break;
        case 2:
        {
            //GUI THREAD
            // Start Threads
            pthread_t thread_id;
            pthread_t GUIthread;
            struct args *mainStruct = (struct args *)malloc(sizeof(struct args));
            mainStruct->matriz = realMatrix;

            pthread_create(&thread_id, NULL, Paint, (void *)mainStruct->matriz);
            pthread_join(thread_id, NULL);

            pthread_create(&GUIthread, NULL, startThreadSolution, realMatrix);
            pthread_join(GUIthread, NULL);
        }
        break;
        case 3:
        {
            pthread_t tid;
            pthread_t GUIthread2;
            pthread_create(&GUIthread2, NULL, Paint, realMatrix2);
            pthread_join(GUIthread2, NULL);

            pthread_create(&tid, NULL, &startForkSolution, realMatrix2);
            pthread_join(tid, NULL);
            break;
        default:
        {
            printf("Opción no válida. \n");
        }
        break;
        }
            /* Fin del anidamiento */
        }
        
    }while (opcion != 4);
        
        
    for (int i = 0; i < realMatrix2->rows; i = i + 1)
        {
            munmap(realMatrix2->matrix_[i], realMatrix2->cols * sizeof(square));
        }
        munmap(realMatrix2->matrix_, realMatrix2->rows * sizeof(square));

    pthread_mutex_destroy(&mutex);

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
    self->finished = (int *)mmap(NULL, 1 * sizeof(int),
                                 PROT_READ | PROT_WRITE,
                                 MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    self->finished = 0;
    return self;
}
matrix *newMatrixFork()
{
    matrix *self = (matrix *)mmap(NULL, sizeof(matrix),
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    self->getMatrixSize = getMatrixSize;
    self->createMatrix = createMatrix;
    self->createMatrixFork = createMatrixFork;
    self->printMatrix = printMatrix;
    self->finished = (int *)mmap(NULL, 1 * sizeof(int),
                                 PROT_READ | PROT_WRITE,
                                 MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    self->finished = 0;
    return self;
}

square *newSquare(int x_, int y_, char t_)
{
    square *self = (square *)malloc(sizeof(square));
    self->down = false;
    self->up = false;
    self->left = false;
    self->right = false;
    self->x = x_;
    self->y = y_;
    self->type = t_;
    self->times = 0;

    return self;
}