#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashTable.h"

int hashCode(int key) {
   return key % SIZE;
}

struct DataItem *search(int key) {
   //get the hash 
   int hashIndex = hashCode(key);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
			
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }        	
   return NULL;        
}

void insert(int key,int data,int exito) {

   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   item->espacios = data;
   item->key = key;
   item->exito = exito;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      //go to next cell
      ++hashIndex;

      //wrap around the table
      hashIndex %= SIZE;
   }
	
   hashArray[hashIndex] = item;
}

struct DataItem* delete(struct DataItem* item) {
   int key = item->key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key) {
         struct DataItem* temp = hashArray[hashIndex]; 
			
         //assign a dummy item at deleted position
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }      
	
   return NULL;        
}

void display() {
   int i = 0;
   for(i = 0; i<SIZE; i++) {
        if(hashArray[i] != NULL)
            printf("THREAD ID: %d, ESPACIOS RECORRIDOS: %d, ESTATUS: %d\n",hashArray[i]->key,hashArray[i]->espacios,hashArray[i]->exito);
   }
}

// int main() {
//    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
//    dummyItem->espacios = -1;  
//    dummyItem->key = -1; 

//    insert(1, 20, 1);
//    insert(2, 70, 1);
//    insert(42, 80,1);
//    insert(4, 25, 1);
//    insert(12, 44, 0);
//    insert(14, 32, 0);
//    insert(17, 11, 1);
//    insert(13, 78, 0);
//    insert(37, 97, 0);

//    display();
//    item = search(37);

// //    if(item != NULL) {
// //       printf("Element found: %d\n", item->data);
// //    } else {
// //       printf("Element not found\n");
// //    }

//  }