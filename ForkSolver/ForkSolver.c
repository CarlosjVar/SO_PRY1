#include "ForkSolver.h"
/*
* 
*
* return: an array with possible directions
*/
void* chooseDirection( matrix *matrix, int filaActual, int colActual,int direction){
    printf("Hola \n");
    //Llegó al objetivo
    if(matrix->finished)
    {
            printf("Finished in index [%d][%d]",filaActual,colActual);
            exit(0);
    }
    if(matrix->matrix_[filaActual][colActual].type =='/'){
            printf("Finished in index [%d][%d]",filaActual,colActual);
            matrix->finished = true ;
            exit(0);
    }

    if(direction == 0 || direction == 1 || direction == -1)
    {
        //Moverse izquierda
        if(colActual-1 >= 0){
            if(matrix->matrix_[filaActual][colActual-1].type!= '*'){

            if(!matrix->matrix_[filaActual][colActual-1].left)
            {
                printf("Voy para la izquierda \n");
                createForkChilds(matrix,filaActual,colActual,2);
            }
            }
        }
        //Moverse derecha
        if(colActual <= matrix->cols-1){
            if(matrix->matrix_[filaActual][colActual+1].type!= '*'){
            printf("Voy para la derecha \n");
            createForkChilds(matrix,filaActual,colActual,3);
            }
        }
    }
    if (direction == 2 || direction == 3 || direction == -1){
        // Moverse abajo
        if(filaActual-1 >= 0){
            if(matrix->matrix_[filaActual-1][colActual].type!= '*'){

            printf("Voy para abajo \n");
            createForkChilds(matrix,filaActual,colActual,0);
            }
        }

        // Moverse arriba
        if(filaActual <= matrix->rows-1){
            if(matrix->matrix_[filaActual+1][colActual].type!= '*'){
            
            printf("Voy para arriba \n");
            createForkChilds(matrix,filaActual,colActual,1);
            }
        }

    }

   
    wait(NULL);
    exit(0);

}

void* travelMatrix(matrix*matrix, int filaActual, int colActual, int direction){

    while(filaActual >= 0 && colActual >= 0 && filaActual < matrix->rows-1 && colActual < matrix->cols-1){
        if(matrix->matrix_[filaActual][colActual].type=='/')
        {
            matrix->finished = true;
            printf("Finished in index [%d][%d]",filaActual,colActual);
            exit(0);
        }
        if(matrix->finished)
        {
            printf("Finished in index [%d][%d]",filaActual,colActual);
            exit(0);
        }
        if(direction == 0){
            if(filaActual==0)
            {
                break;
            }
            if(matrix->matrix_[filaActual-1][colActual].type == '*')
            {
                printf("Chocó");
                break;
            }
            filaActual--;
            matrix->matrix_[filaActual][colActual].down==true;
        }
        else if(direction == 1){
            if(matrix->matrix_[filaActual+1][colActual].type == '*')
            {
                printf("Chocó");
                break;
            }
            filaActual++;
            matrix->matrix_[filaActual][colActual].up==true;
        }
        else if(direction == 2){
            if(matrix->matrix_[filaActual][colActual-1].type == '*')
            {   printf("Chocó");
                break;
            }
            colActual--;
            matrix->matrix_[filaActual][colActual].left==true;
        }
        else if (direction == 3){
            if(matrix->matrix_[filaActual][colActual+1].type == '*')
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
    chooseDirection(matrix,filaActual,colActual,direction);
}
void* createForkChilds(struct matrix*matrix, int filaActual, int colActual,int direction){

        pid_t pid= fork();
        if(pid==0)
        {
            printf("Se creó un hijo \n");
            travelMatrix(matrix,filaActual,colActual,direction);
        }



}