//Evan Hughes [eshughes@ucsc.edu]

#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
#include "List.h"
#include "insertionSort.h"
#include "heapSort.h"

/* opens and reads a file, creates an array of integers based on the input file, then sorts them with both 
   insertion sort and heap sort, prints out the array prior to sorting and post-sorting for both algorithms
*/
void openFile(char *filename){
	FILE *input = fopen(filename, "r");  //open the file
	int numKeys = 0; 
	if(input == NULL){
		printf("error: no input file\n");
	} else {
		fscanf(input, "%d", &numKeys); 
	}
	
	int testArray[numKeys];

	for(int i = 0; i < numKeys; i++){
		int key;
		fscanf(input, "%d", &key);
		testArray[i] = key;
	}

	int arrayCopy[numKeys]; //array holding original order of keys

	printf("Array prior to sorting: \nIndex : Key \n");
	for(int i = 0; i < numKeys; i++){
		printf(" %d   :   %d \n", i, testArray[i]);
		arrayCopy[i] = testArray[i];
	}

	printf("Array post-insertionSort: \nIndex : Key \n");
	insertionSort(testArray, numKeys);
	for(int i = 0; i < numKeys; i++){
		printf(" %d   :   %d \n", i, testArray[i]);
	}

	printf("Array post-heapSort: \nIndex : Key \n");
	heapSort(arrayCopy, numKeys);
	for(int i = 0; i < numKeys; i++){
		printf(" %d   :   %d \n", i, arrayCopy[i]);
	}
	fclose(input);  //close the file we opened
}

int main(int argc, char **argv){
	if(argc == 1){
	} else {
		for(int i = 1; i < argc; i++){
			openFile(argv[i]);
		}
	}
}
