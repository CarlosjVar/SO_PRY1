
#include <stdio.h>
#include "Matrix/matrix.h"
#include "FileReader/fileReader.h"
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ANSI_COLOR_WALL "\x1B[38;2;37;92;87m"
#define ANSI_COLOR_PATH "\x1B[38;2;247;197;146m"
#define ANSI_COLOR_PATH_2 "\x1B[38;2;255;87;51m"
#define ANSI_COLOR_PATH_3 "\x1B[38;2;199;0;57m"
#define ANSI_COLOR_PATH_4 "\x1B[38;2;74;6;32m"
#define ANSI_COLOR_GOAL "\x1B[38;2;97;164;124m"
#define ANSI_COLOR_RESET   "\x1b[0m"
pthread_mutex_t mutex;

void printMatrix(matrix *self)
{
    system("clear");
    for (int i = 0; i < self->rows; i = i + 1)
    {
        for (int j = 0; j < self->cols; j = j + 1)
        {
            pthread_mutex_lock(&mutex);
            square actual = self->matrix_[i][j];
            pthread_mutex_unlock(&mutex);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct args {
    struct matrix *matriz;
    int filaAct;
    int colAct;
    int dirAct;
};

//Hay que eliminar parametros cantfilas y columnas y agregar condicion de no haber pasado por ahi al integrar con struct
//Orden de Array direccion es : arriba abajo izq der salida
int* elegirDireccion(struct matrix *self, int filaActual, int colActual, int dirActual){
    int *direcciones;    
    direcciones = (int *)malloc(5*sizeof (int));
    direcciones[0] = 0;
    direcciones[1] = 0;
    direcciones[2] = 0;
    direcciones[3] = 0;
    direcciones[4] = 0;
    if(self->matrix_[filaActual][colActual].type == '/'){
            direcciones[4] = 5;
        }

    if(dirActual != 0 && dirActual != 1)
    {
        if(filaActual-1 >= 0){
            if(self->matrix_[filaActual-1][colActual].type != '*' && !self->matrix_[filaActual][colActual].up){
                direcciones[0] = 5;
            }
        }
        if(filaActual+1 < self->rows){
            if(self->matrix_[filaActual+1][colActual].type != '*' && !self->matrix_[filaActual][colActual].down){
                direcciones[1] = 5;
            }
        }
    }
    if(dirActual != 2 && dirActual != 3)
    {
        if(colActual-1 >= 0){
            if(self->matrix_[filaActual][colActual-1].type != '*' && !self->matrix_[filaActual][colActual].left){
                direcciones[2] = 5;
            }
        }
        if(colActual+1 < self->cols){
            if(self->matrix_[filaActual][colActual+1].type != '*' && !self->matrix_[filaActual][colActual].right){
                direcciones[3] = 5;
            }
        }
    }
    return direcciones;
}

//0 es arriba, 1 es abajo, 2 es izq, 3 es der
//(void *arg,struct matrix *self, int filaActual, int colActual, int direccion)
void* realKeepGoing(void * currentStruct){
    int threadCounter;
    pthread_mutex_lock(&mutex);
    int filaActual = ((struct args*)currentStruct)->filaAct;
    int colActual = ((struct args*)currentStruct)->colAct;
    int direccion = ((struct args*)currentStruct)->dirAct;
    struct matrix * self = ((struct args*)currentStruct)->matriz;
    int rowNum = self->rows;
    int colNum = self->cols;
    pthread_mutex_unlock(&mutex);
    if(rowNum>colNum){
        threadCounter = rowNum;
    }
    else{
        threadCounter = colNum;
    }
    pthread_t tid[threadCounter];
    threadCounter = 0;
    int *dirs;
    while(filaActual >= 0 && colActual >= 0 && filaActual < rowNum && colActual < colNum){
        self->printMatrix(self);
        printf("\n");
        sleep(1);
        if(direccion == 0){
            pthread_mutex_lock(&mutex);
            self->matrix_[filaActual][colActual].up = true;
            pthread_mutex_unlock(&mutex);
            filaActual--;
        }
        else if(direccion == 1){
            pthread_mutex_lock(&mutex);
            self->matrix_[filaActual][colActual].down = true;
            pthread_mutex_unlock(&mutex);
            filaActual++;
        }
        else if(direccion == 2){
            pthread_mutex_lock(&mutex);
            self->matrix_[filaActual][colActual].left = true;
            pthread_mutex_unlock(&mutex);
            colActual--;
        }
        else if(direccion == 3){
            pthread_mutex_lock(&mutex);
            self->matrix_[filaActual][colActual].right = true;
            pthread_mutex_unlock(&mutex);
            colActual++;
        }
        if(filaActual < 0 || colActual < 0 || filaActual >= rowNum || colActual >= colNum){
            threadCounter--;
                for(threadCounter;threadCounter>=0;threadCounter--){
                    pthread_join(tid[threadCounter],NULL);
                }
                break;
        }
        if (self->matrix_[filaActual][colActual].type == '*' || self->matrix_[filaActual][colActual].type == '/' || direccion == 6){
                threadCounter--;

                for(threadCounter;threadCounter>=0;threadCounter--){
                    pthread_join(tid[threadCounter],NULL);
                }
                break;
        }

        self->matrix_[filaActual][colActual].times++;

        pthread_mutex_lock(&mutex);
        dirs = elegirDireccion(self, filaActual, colActual, direccion);
        pthread_mutex_unlock(&mutex);

        if(dirs[0] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&mutex);
            newStruct->matriz = self;
            pthread_mutex_unlock(&mutex);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 0;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing, newStruct);
            threadCounter++;
        }
        if(dirs[1] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&mutex);
            newStruct->matriz = self;
            pthread_mutex_unlock(&mutex);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 1;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing, newStruct);
            threadCounter++;
        }
        if(dirs[2] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&mutex);
            newStruct->matriz = self;
            pthread_mutex_unlock(&mutex);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 2;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing,newStruct);
            threadCounter++;
        }
        if(dirs[3] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&mutex);
            newStruct->matriz = self;
            pthread_mutex_unlock(&mutex);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 3;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing,newStruct);
            threadCounter++;
        }
        if(direccion == 5){
            direccion++;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    fileReader *reader = newFileReader();
    reader->readFileLen(reader);
    
    struct matrix *realMatrix = newMatrix();
    //struct matrix *realMatrix2 = newMatrix();

    realMatrix->getMatrixSize(reader->linelen, realMatrix);
    realMatrix->path = reader->matrix_;
    //realMatrix2 = realMatrix;

    realMatrix->createMatrix(realMatrix);
    //realMatrix2->createMatrixFork(realMatrix2);
    // realMatrix->printMatrix(realMatrix);
    
    // int* direcciones = elegirDireccion(realMatrix,0,2,);
    // printf(" %d",direcciones[0]);
    // printf(" %d",direcciones[1]);
    // printf(" %d",direcciones[2]);
    // printf(" %d",direcciones[3]);
    // printf(" %d",direcciones[4]);
    
    struct args *mainStruct = (struct args *)malloc(sizeof(struct args));
    mainStruct->matriz = realMatrix;
    mainStruct->filaAct = 0;
    mainStruct->colAct = 0;
    mainStruct->dirAct = 5;

    pthread_t mainthread;
    pthread_create(&mainthread,NULL,realKeepGoing, (void *)mainStruct);
    pthread_join(mainthread, NULL);

    realMatrix->printMatrix(realMatrix);
    
    printf("\n Final");

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

