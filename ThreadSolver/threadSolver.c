#include "threadSolver.h"
#include "pthread.h"
#include "matrix.h"
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