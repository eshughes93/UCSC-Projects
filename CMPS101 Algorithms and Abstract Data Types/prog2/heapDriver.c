//Evan Hughes [eshughes@ucsc.edu]
#include<stdio.h>
#include<stdlib.h>
#include "heap.h"

/*opens files specified by standard in
  creates an array based on contents of file input
  inserts array into heap, sorts accordingly. 
*/
void openFile(char *filename){	
	FILE *input = fopen(filename, "r");
	int heapSize = 0;
	if(input == NULL){
		printf("error\n");
	} else {
		fscanf(input, "%d", &heapSize);		//initialize heapSize with the specified amount of keys to be sorted 
	}
	int testArray[heapSize]; //create array of size heapSize

	printf("heapSize is %d\n", heapSize);
	heapRef test = newHeap(heapSize);	//new heap with a max size of heapSize
	if(emptyHeap(test)){
		printf("The heap is empty before insertion.\n");	//test heap emptiness, as it should be empty before putting anything in
	}
	for(int i = 0; i < heapSize; i++){
		int key;
		fscanf(input, "%d", &key);
		testArray[i] = key;
		insert(test, key);
	}
	heapRef testTwo = buildHeap(heapSize, testArray, heapSize);   //second heap to test buildHeap function
	printf("The heap after insertion and heapSort: \n");
	printHeap(test);
	//printHeap(testTwo);

	if(isFull(testTwo)){
		printf("The heap is full as expected.\n");	//test heap fullness, should be full after sorting heapSize objects in a heap with a maxSize of heapSize
	}
	printf("the max value in the heap is %d \n", maxValue(test));  //print maxValue and test maxValue function
	printf("deleting max value...\n");
	deleteMax(test);	//testing deleteMax
	printHeap(test);	//printing to show that deleteMax worked
	freeHeap(test);
	freeHeap(testTwo);
	fclose(input);	//close input file
}

int main(int argc,char **argv){
	if(argc == 1){
	} else {
		for(int i = 1; i < argc; i++){
			openFile(argv[i]);
		}
	}
}
