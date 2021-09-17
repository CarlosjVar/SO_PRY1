#include "ForkSolver.h"
/*
* 
*
* return: an array with possible directions
*/
void* chooseDirection( matrix *matrix, int filaActual, int colActual){
    int *direcciones;
    direcciones = (int *)malloc(5);
    //Llegó al objetivo
    if(matrix->matrix_[filaActual][colActual].type =='/'){
            matrix->finished = true ;
        }

    // Moverse abajo
    if(filaActual-1 >= 0){
        if(matrix->matrix_[filaActual-1][colActual].type!= '*'){
            printf('\033[0;31m');
            printf("Voy para abajo");
            //createForkChilds(matrix,filaActual,colActual,0);
        }
    }
    // Moverse arriba
    if(filaActual+1 < matrix->rows){
        if(matrix->matrix_[filaActual+1][colActual].type!= '*'){
        printf("Voy para arriba");
            //createForkChilds(matrix,filaActual,colActual,1);
        }
    }
    //Moverse izquierda
    if(colActual-1 >= 0){
        if(matrix->matrix_[filaActual][colActual-1].type!= '*'){
        printf("Voy para la izquierda");
            //createForkChilds(matrix,filaActual,colActual,2);
        }
    }
    //Moverse derecha
    if(colActual+1 < matrix->cols){
        if(matrix->matrix_[filaActual][colActual+1].type!= '*'){
        printf("Voy para la derecha");
           //createForkChilds(matrix,filaActual,colActual,3);
        }
    }

    wait();

    return ;
}

void* travelMatrix(struct matrix*matrix, int filaActual, int colActual, int direccion){

    while(filaActual >= 0 && colActual >= 0 && filaActual < matrix->rows && colActual < matrix->cols){

        if (matrix->finished);
            exit()
        if(direccion == 0){
            filaActual--;
            matrix->matrix_[filaActual][colActual].down==true;
        }
        else if(direccion == 1){
            filaActual++;
            matrix->matrix_[filaActual][colActual].up==true;
        }
        else if(direccion == 2){
            colActual--;
            matrix->matrix_[filaActual][colActual].left==true;
        }
        else if(direccion == 3){
            colActual++;
            matrix->matrix_[filaActual][colActual].right==true;
        }
      
    }
    // Se deben crear forks para continuar ya que se topó con una pared
}
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual,int direction){

        pid_t pid= fork();
        if(pid==0)
        {
            travelMatrix(matrix,filaActual,colActual,direction);
        }



}