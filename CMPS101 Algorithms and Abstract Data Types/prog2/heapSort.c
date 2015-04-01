//Evan Hughes [eshughes@ucsc.edu]
#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
#include "heapSort.h"

//performs heapSort algorithm upon a given array, user can see changes in passed array
void heapSort(int keys[], int numKeys){
	heapRef sort = buildHeap(numKeys, keys, numKeys);
	for(int i = numKeys-1; i > 0; i--){
		keys[i] = maxValue(sort);
		deleteMax(sort);
	}
	keys[0] = maxValue(sort);
	freeHeap(sort);
}

