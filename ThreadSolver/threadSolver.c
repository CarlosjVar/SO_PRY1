#include "threadSolver.h"
void* realKeepGoing(void * currentStruct){
    int threadCounter;
    pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
    int filaActual = ((struct args*)currentStruct)->filaAct;
    int colActual = ((struct args*)currentStruct)->colAct;
    int direccion = ((struct args*)currentStruct)->dirAct;
    int camino = ((struct args*)currentStruct)->camRecorrido;
    struct matrix * self = ((struct args*)currentStruct)->matriz;
    int rowNum = self->rows;
    int colNum = self->cols;
    pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
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
        printf("\n\n");
        sleep(1);
        if(direccion == 0){
            if(self->matrix_[filaActual][colActual].up)
            {
                direccion = 6;
                continue;
            }
            camino++;
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            self->matrix_[filaActual][colActual].up = true;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            filaActual--;
        }
        else if(direccion == 1){
            if(self->matrix_[filaActual][colActual].down)
            {
                direccion = 6;
                continue;
            }
            camino++;
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            self->matrix_[filaActual][colActual].down = true;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            filaActual++;
        }
        else if(direccion == 2){
            if(self->matrix_[filaActual][colActual].left)
            {
                direccion = 6;
                continue;
            }
            camino++;
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            self->matrix_[filaActual][colActual].left = true;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            colActual--;
        }
        else if(direccion == 3){
            if(self->matrix_[filaActual][colActual].right)
            {
                direccion = 6;
                continue;
            }
            camino++;
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            self->matrix_[filaActual][colActual].right = true;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
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
                if(self->matrix_[filaActual][colActual].type == '/'){
                    self->matrix_[filaActual][colActual].times++;
                    printf("Salida encontrada! Camino recorrido:%d",camino);
                     printf("\n");    
                }
                else if(self->matrix_[filaActual][colActual].type == '*'){
                    printf("\n");
                }
                for(threadCounter;threadCounter>=0;threadCounter--){
                    pthread_join(tid[threadCounter],NULL);
                }
                break;
        }

        self->matrix_[filaActual][colActual].times++;

        pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
        dirs = elegirDireccion(self, filaActual, colActual, direccion);
        pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);

        if(dirs[0] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->matriz = self;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 0;
            newStruct->camRecorrido = camino;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing, newStruct);
            threadCounter++;
        }
        if(dirs[1] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->matriz = self;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 1;
            newStruct->camRecorrido = camino;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing, newStruct);
            threadCounter++;
        }
        if(dirs[2] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->matriz = self;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 2;
            newStruct->camRecorrido = camino;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing,newStruct);
            threadCounter++;
        }
        if(dirs[3] == 5){
            struct args *newStruct = (struct args *)malloc(sizeof(struct args));
            pthread_mutex_lock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->matriz = self;
            pthread_mutex_unlock(&((struct args*)currentStruct)->matriz->lock);
            newStruct->filaAct = filaActual;    
            newStruct->colAct = colActual;
            newStruct->dirAct = 3;
            newStruct->camRecorrido = camino;
            pthread_create(&(tid[threadCounter]), NULL, realKeepGoing,newStruct);
            threadCounter++;
        }
        if(direccion == 5){
            direccion++;
        }
    }
}

int* elegirDireccion(struct matrix *self, int filaActual, int colActual, int dirActual){
    int *direcciones;    
    direcciones = (int *)malloc(5*sizeof (int));
    direcciones[0] = 0;
    direcciones[1] = 0;
    direcciones[2] = 0;
    direcciones[3] = 0;
    direcciones[4] = 0;
    //Condicion nunca entra
    if(self->matrix_[filaActual][colActual].type == '/'){
        printf("Llegó");
        self->matrix_[filaActual][colActual].times++;
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