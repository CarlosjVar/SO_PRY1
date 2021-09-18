#include "ForkSolver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include<sys/wait.h>
#include <unistd.h>
#include <pthread.h>
/*
* 
*
* return: an array with possible directions
*/
void* chooseDirection( matrix *matrix, int filaActual, int colActual,int direction){

    
    //Llegó al objetivo
    if(matrix->finished)
    {
            printf("Finished in index [%d][%d]",filaActual,colActual);
            _Exit(getpid());
    }
    if(matrix->matrix_[filaActual][colActual].type =='/'){
            matrix->matrix_[filaActual][colActual].times++;
            matrix->finished = true ;
            return;
    }

    if(direction == 0 || direction == 1 || direction == -1)
    {
        //Moverse izquierda
        if(colActual-1 >= 0){
            if(matrix->matrix_[filaActual][colActual-1].type!= '*'){

            if(!matrix->matrix_[filaActual][colActual].left)
            {
                createForkChilds(matrix,filaActual,colActual,2);
            }
            }
        }
        //Moverse derecha
        if(colActual <= matrix->cols){
            if(matrix->matrix_[filaActual][colActual+1].type!= '*'){
                if(!matrix->matrix_[filaActual][colActual].right)
                {
                    createForkChilds(matrix,filaActual,colActual,3);
                }
            }
        }
    }
    if (direction == 2 || direction == 3 || direction == -1){
        // Moverse abajo
        if(filaActual-1 >= 0){
            if(matrix->matrix_[filaActual-1][colActual].type!= '*'){
                
                if(!matrix->matrix_[filaActual][colActual].down)
                {
                    createForkChilds(matrix,filaActual,colActual,0);
                }
            }
        }

        // Moverse arriba
        if(filaActual <= matrix->rows){
            if(matrix->matrix_[filaActual+1][colActual].type!= '*'){
                if(!matrix->matrix_[filaActual][colActual].up)
                {
                    createForkChilds(matrix,filaActual,colActual,1);
                }
            }
        }

    }
}

void* travelMatrix(matrix*matrix, int filaActual, int colActual, int direction){

    while(filaActual >= 0 && colActual >= 0 && filaActual < matrix->rows && colActual < matrix->cols){
    
        if(direction == 0){
            pthread_mutex_lock(&matrix->lock);
            if(filaActual==0)
            {
                break;
            }
            if(matrix->matrix_[filaActual-1][colActual].type == '*')
            {
                break;
            }
            matrix->matrix_[filaActual][colActual].times++;
            matrix->matrix_[filaActual][colActual].down==true;
            filaActual--;
            pthread_mutex_unlock(&matrix->lock);
        }
        else if(direction == 1){
            printf("Estoy en la hijueputa fila %d y en el re malparido %d \n",filaActual,colActual);
            pthread_mutex_lock(&matrix->lock);
            if(matrix->matrix_[filaActual+1][colActual].type == '*')
            {
                break;
            }
            matrix->matrix_[filaActual][colActual].times++;
            matrix->matrix_[filaActual][colActual].up==true;
            filaActual++;
            pthread_mutex_unlock(&matrix->lock);
        }   
        else if(direction == 2){
            pthread_mutex_lock(&matrix->lock);
            if(colActual==0)
            {
                break;
            }
            if(matrix->matrix_[filaActual][colActual-1].type == '*')
            {
                break;
            }
            matrix->matrix_[filaActual][colActual].times++;
            matrix->matrix_[filaActual][colActual].left==true;
            colActual--;
            pthread_mutex_unlock(&matrix->lock);
        }
        else if (direction == 3){
            pthread_mutex_lock(&matrix->lock);
            if(matrix->matrix_[filaActual][colActual+1].type == '*')
            {
                break;
            }
            matrix->matrix_[filaActual][colActual].times++;
            matrix->matrix_[filaActual][colActual].right==true;
            colActual++;
            pthread_mutex_unlock(&matrix->lock);
            }

        if(filaActual == 4 && colActual == 3)
        {
            printf("El caracter de la celda es");

        }
        sleep(2);
        chooseDirection(matrix,filaActual,colActual,direction);
        matrix->printMatrix(matrix);
       
      
    }
    //matrix->printMatrix(matrix);

    _Exit(getpid());
    // Se deben crear forks para continuar ya que se topó con una pared
}
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual,int direction){

        pid_t pid= fork();
        if(pid==0)
        {
            travelMatrix(matrix,filaActual,colActual,direction);
        }



}