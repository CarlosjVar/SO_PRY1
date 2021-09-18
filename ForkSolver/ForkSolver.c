#include "ForkSolver.h"


/*
* 
*
* return: an array with possible directions
*/
int* selectDirection(struct matrix *self, int filaActual, int colActual, int dirActual){
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
void* travelMatrix(matrix*matrix, int filaActual, int colActual, int direction){

    pthread_mutex_lock(&(matrix->lock));

    int direccion = direction;
    int rowNum = matrix->rows;
    int colNum = matrix->cols;
    pthread_mutex_unlock(&(matrix->lock));


    int *dirs;
    while(filaActual >= 0 && colActual >= 0 && filaActual < rowNum && colActual < colNum){
        //self->printMatrix(self);
        printf("\n");
        sleep(1);
        if(direccion == 0){
            pthread_mutex_lock(&(matrix->lock));
            matrix->matrix_[filaActual][colActual].up = true;
            pthread_mutex_unlock(&(matrix->lock));
            filaActual--;
        }
        else if(direccion == 1){
            pthread_mutex_lock(&(matrix->lock));
            matrix->matrix_[filaActual][colActual].down = true;
            pthread_mutex_unlock(&(matrix->lock));
            filaActual++;
        }
        else if(direccion == 2){
            pthread_mutex_lock(&(matrix->lock));
            matrix->matrix_[filaActual][colActual].left = true;
            pthread_mutex_unlock(&(matrix->lock));
            colActual--;
        }
        else if(direccion == 3){
            pthread_mutex_lock(&(matrix->lock));
            matrix->matrix_[filaActual][colActual].right = true;
            pthread_mutex_unlock(&(matrix->lock));
            colActual++;
        }
        if(filaActual < 0 || colActual < 0 || filaActual >= rowNum || colActual >= colNum){
                //End process
               _exit(0);
                break;
        }
        if (matrix->matrix_[filaActual][colActual].type == '*' || matrix->matrix_[filaActual][colActual].type == '/' || direccion == 6){
                // End process
                if(matrix->matrix_[filaActual][colActual].type == '/'){
                    matrix->matrix_[filaActual][colActual].times++;
                }
               _exit(0);
                break;
        }

        matrix->matrix_[filaActual][colActual].times++;

        pthread_mutex_lock(&(matrix->lock));
        dirs = selectDirection(matrix, filaActual, colActual, direccion);
        pthread_mutex_unlock(&(matrix->lock));

        if(dirs[0] == 5){
            createForkChilds(matrix,filaActual,colActual,0);
        }
        if(dirs[1] == 5){
            createForkChilds(matrix,filaActual,colActual,1);
        }
        if(dirs[2] == 5){
            createForkChilds(matrix,filaActual,colActual,2);
        }
        if(dirs[3] == 5){
            createForkChilds(matrix,filaActual,colActual,3);
        }
        if(direccion == 5){
            direccion++;
        }
        matrix->printMatrix(matrix);
    }

}
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual,int direction){

        pid_t pid= fork();
        printf("El fork devolvió %d \n ",pid);
        if(pid==0)
        {
            travelMatrix(matrix,filaActual,colActual,direction);
            _exit(0);
        }



}