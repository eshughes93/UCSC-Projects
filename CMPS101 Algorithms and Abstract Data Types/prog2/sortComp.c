//Evan Hughes [eshughes@ucsc.edu]
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
#include "List.h"
#include "insertionSort.h"
#include "heapSort.h"

//returns the clock_t time it takes for insertionSort to be performed on a given array data[] with numKeys values
clock_t insertionSortTime(int data[], int numKeys){
	clock_t startTime;
	startTime = clock();
	insertionSort(data, numKeys);
	return (clock() - startTime);
}
//returns the clock_t time it takes for heapSort to be performed on a given array data[] with numKeys values
clock_t heapSortTime(int data[], int numKeys){
	clock_t startTime;
	startTime = clock();
	heapSort(data, numKeys);
	return (clock() - startTime);
}

//takes input file, creates array of integers based on that file, prints time comparisons for insertsionSort and heapSort algorithms
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

	for(int i = 0; i < numKeys; i++){
		arrayCopy[i] = testArray[i]; //make a copy of the original array so heapSort doesn't sort an already sorted array
	}

	printf("\nSorting %d elements. \n", numKeys);

	float insertionSortDuration =  ((float)insertionSortTime(testArray, numKeys)/CLOCKS_PER_SEC);
	float heapSortDuration =  ((float)heapSortTime(arrayCopy, numKeys)/CLOCKS_PER_SEC);

	printf("Insertion sort took %f seconds. \n", insertionSortDuration);
	printf("Heap sort took %f seconds. \n", heapSortDuration);
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

