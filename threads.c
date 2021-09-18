#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matrix.h"


void* hilo1 (void * arg){
    int x = 0;
    while(1){

        printf("Hilo 1 %u\n", x);
        x++;

    }
    return NULL;
}

void* hilo2 (void * arg){
    int y = 0;
    while(1){

        printf("Hilo 2 %u\n", y);
        y++;
    }
    return NULL;
}

void nuevoThread(){    
    pthread_t nuevohilo;
    pthread_create(&nuevohilo,NULL,hilo2,NULL);
}

void printMatrix(int**Matrix,int rows,int columns)
{
    for (int i = 0; i < rows; ++i)
    {
        printf("\n");
         for (int j = 0; j < columns; ++j)
        {
          printf(" %d", Matrix[i][j]);
        }
    }
}

//Hay que eliminar parametros cantfilas y columnas y agregar condicion de no haber pasado por ahi al integrar con struct
//Orden de Array direccion es : arriba abajo izq der salida
int* elegirDireccion(int** matriz, int filaActual, int colActual, int cantFilas, int cantColumnas){
    int *direcciones;
    direcciones = (int *)malloc(5);

    if(matriz[filaActual][colActual] == 2){
            direcciones[4] = 5;
        }

    if(filaActual-1 >= 0){
        if(matriz[filaActual-1][colActual] != 0){
            direcciones[0] = 5;
        }
    }
    if(filaActual+1 < cantFilas){
        if(matriz[filaActual+1][colActual] != 0){
            direcciones[1] = 5;
        }
    }
    if(colActual-1 >= 0){
        if(matriz[filaActual][colActual-1] != 0){
            direcciones[2] = 5;
        }
    }
    if(colActual+1 < cantColumnas){
        if(matriz[filaActual][colActual+1] == 1){
            direcciones[3] = 5;
        }
    }

    return direcciones;
}

//Aun no devuelve matriz
int* keepGoing(int** matriz, int filaActual, int colActual, int direccion, int cantFilas, int cantColumnas){
    int *posFinal;
    posFinal = (int *)malloc(2);

    while(filaActual >= 0 && colActual >= 0 && filaActual < cantFilas && colActual < cantColumnas){

        if(direccion == 0){
            filaActual--;
        }
        if(direccion == 1){
            filaActual++;
        }
        if(direccion == 2){
            colActual--;
        }
        if(direccion == 3){
            colActual++;
        }
        if (matriz[filaActual][colActual]==0 || matriz[filaActual][colActual]== 2 || matriz[filaActual][colActual]== 9){
            //printMatrix(matriz,9,9);
            return posFinal;
        }
        posFinal[0] = filaActual;
        posFinal[1] = colActual;
        matriz[filaActual][colActual] = 5;
    }
}


int main(int argc, char argv[]){
    // while(1){

    // // pthread_t hilo;
    // // pthread_create(&hilo,NULL,hilo1,NULL);



    // pthread_t hilo;
    // pthread_create(&hilo,NULL,hilo1,NULL);

    // nuevoThread();

    int i,j,lines,columns;
    printf("Type the matrix lines: \n");
    scanf("%d", &lines);
    printf("Type the matrix columns: \n");
    scanf("%d", &columns);
    int **intMatrix;
    intMatrix = (int **) malloc(lines*sizeof(int*));
    for (i = 0; i < lines; ++i)
      intMatrix[i] = (int *)malloc(columns * sizeof(int)); 

    //9 es inicio, 1 camino, 0 pared y 5 camino recorrido
    intMatrix[0][4] = 9;
    intMatrix[1][4] = 1;
    intMatrix[2][4] = 1;
    intMatrix[2][3] = 1;
    intMatrix[2][5] = 1;
    intMatrix[2][2] = 1;
    intMatrix[2][6] = 1;
    intMatrix[3][2] = 1;
    intMatrix[3][6] = 1;
    intMatrix[3][7] = 1;
    intMatrix[3][1] = 1;
    intMatrix[3][0] = 2;
    intMatrix[3][8] = 1;
    intMatrix[4][2] = 1;
    intMatrix[4][6] = 1;
  


    printMatrix(intMatrix,lines,columns);
    printf("\n\n");

    // int* direcciones = elegirDireccion(intMatrix,0,4,9,9);

    // printf(" %d",direcciones[0]);
    // printf(" %d",direcciones[1]);
    // printf(" %d",direcciones[2]);
    // printf(" %d",direcciones[3]);
    // printf(" %d",direcciones[4]);

    int *lastPos = keepGoing(intMatrix,3,2,2,9,9);
    
    printMatrix(intMatrix,lines,columns);
    printf("\n ");
    printf(" %d",lastPos[0]);
    printf(" %d",lastPos[1]);

    printf("\n\nFunca\n");

    //};
    //return 0;
}

// //0 es arriba, 1 es abajo, 2 es izq, 3 es der
// int* keepGoing(struct matrix *self, int filaActual, int colActual, int direccion){
//     int *posFinal;
//     posFinal = (int *)malloc(2);

//     while(filaActual >= 0 && colActual >= 0 && filaActual < self->rows && colActual < self->cols){

//         if(direccion == 0){
//             filaActual--;
//         }
//         if(direccion == 1){
//             filaActual++;
//         }
//         if(direccion == 2){
//             colActual--;
//         }
//         if(direccion == 3){
//             colActual++;
//         }
//         if (self->matrix_[filaActual][colActual].type == '*' || self->matrix_[filaActual][colActual].type == '/'){
//             //printMatrix(matriz,9,9);
//             return posFinal;
//         }
//         posFinal[0] = filaActual;
//         posFinal[1] = colActual;
//         self->matrix_[filaActual][colActual].times++;
//     }
// }
