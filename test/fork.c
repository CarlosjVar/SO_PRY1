#include "stdio.h"
#include <sys/wait.h>
#include <sys/mman.h>

//Doesn't take in count already traveled paths
int getDirection(int**Matrix,int currentRow,int currentColumn,int rows,int columns){

  if(currentRow<rows && Matrix[currentRow+1][currentColumn] != 1 )
  {
    printf("Se eligió dirección Norte, posición actual fila %d columna %d \n",currentRow,currentColumn);
    return 1;
  }
  else if(currentColumn<columns && Matrix[currentRow][currentColumn+1] !=1  )
  {
    printf("Se eligió dirección Este, posición actual fila %d columna %d \n",currentRow,currentColumn);
    return 2;
  }
  else if(currentRow<0 && Matrix[currentRow-1][currentColumn] != 1 )
  {
    printf("Se eligió dirección Sur, posición actual fila %d columna %d \n",currentRow,currentColumn);
    return 3;
  }
  else if(currentColumn>0 && Matrix[currentRow][currentColumn-1] != 1 )
  {
    printf("Se eligió dirección Oeste, posición actual fila %d columna %d \n",currentRow,currentColumn);
    return 4;
  }
  else
  {
    return 0;
  }

}

void travelMatrix(int**intMatrix,int direction,int currentRow,int currentColumn,int rows,int columns)
{
  //printMatrix(intMatrix,rows,columns);
  if(direction==0){

    printf("No hay caminos");
    return;
  }
  if( intMatrix[currentRow][currentColumn] == 2)
  {
    printf("Llego");
    printMatrix(intMatrix,rows,columns);
  }
  printMatrix(intMatrix,rows,columns);
  if(direction == 1 ){
    for (currentRow; currentRow<rows;currentRow++)
        {
          intMatrix[currentRow][currentColumn] = 3;

          if(currentRow==rows){
            break;
          }
          else if (intMatrix[currentRow+1][currentColumn] == 1)
          {
            break;
          }
          
        }

  }
  else if(direction == 2)
  {
      for (currentColumn; currentColumn<columns;currentColumn++)
      {
        printf("Columna %d",currentColumn);
        intMatrix[currentRow][currentColumn] = 3;
        if ( (currentColumn==columns) )
        {
          break;
        }
        else if(intMatrix[currentRow][currentColumn+1] == 1 )
        {
          break;
        }
        
      }
  }
  else if(direction == 3)
  {
    for (currentRow; currentRow>0;currentRow--)
    {
      intMatrix[currentRow][currentColumn] = 3;
      if (intMatrix[currentRow-1][currentColumn] == 1)
      {
        break;
      }
      
    }
  }
  else if(direction == 4)
  {
      for (currentColumn; currentColumn>0;currentColumn--)
      {
        intMatrix[currentRow][currentColumn] = 3;
        if (intMatrix[currentRow][currentColumn-1] == 1)
        {
          break;
        }
        
      }
  }
  //Creates new child process with new direction
  int child = fork();
  if(child == 0)
  { 

    int newDirection = getDirection(intMatrix,currentRow,currentColumn,rows,columns);
    printf("Tiramos nuevo proceso lesgo, se dirige a %d \n",newDirection);
    travelMatrix(intMatrix,newDirection,currentRow,currentColumn,rows,columns);
  }
  
}

int main(int argc, char argv[])
{   
    // int i,j,lines,columns;
    // printf("Type the matrix lines:\t");
    // scanf("%d", &lines);
    // printf("Type the matrix columns:\t");
    // scanf("%d", &columns);
    // int **intMatrix;
    // intMatrix = (int **) malloc(lines*sizeof(int*));
    // for (i = 0; i < lines; ++i)
    //   intMatrix[i] = (int *)malloc(columns * sizeof(int)); 

    // intMatrix[9][8] = 1;
    // intMatrix[0][9] = 1;
    // intMatrix[8][0] = 1;
    // intMatrix[1][0] = 2;
    // int direction = 2;
    // int currentRow,currentColumn=0;
    // travelMatrix(intMatrix,direction,currentRow,currentColumn,lines,columns);

     int N=5; // Number of elements for the array
   
    int *ptr = mmap(NULL,N*sizeof(int),
     PROT_READ | PROT_WRITE,
     MAP_SHARED | MAP_ANONYMOUS,
     0,0);    

    if(ptr == MAP_FAILED){
     printf("Mapping Failed\n");
     return 1;
    }

    for(int i=0; i < N; i++){
     ptr[i] = i + 1;
    }

    printf("Initial values of the array elements :\n");
    for (int i = 0; i < N; i++ ){
     printf(" %d", ptr[i] );
    }
    printf("\n");

    pid_t child_pid = fork();
   
    if ( child_pid == 0 ){
     //child
     for (int i = 0; i < N; i++){
         ptr[i] = ptr[i] * 10;
     }
    }
    else{
     //parent
     waitpid ( child_pid, NULL, 0);
     printf("\nParent:\n");

     printf("Updated values of the array elements :\n");
     for (int i = 0; i < N; i++ ){
         printf(" %d", ptr[i] );
     }
     printf("\n");
    }

    int err = munmap(ptr, N*sizeof(int));

    if(err != 0){
     printf("UnMapping Failed\n");
     return 1;
    }
    return 0;
}


void printMatrix(int**Matrix,int rows,int columns)
{
    for (int i = 0; i < rows; ++i)
    {
        printf("\n");
         for (int j = 0; j < columns; ++j)
        {
          printf("%d", Matrix[i][j]);
        }
    }
}