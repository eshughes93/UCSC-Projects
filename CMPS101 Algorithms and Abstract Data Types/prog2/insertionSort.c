//Evan Hughes [eshughes@ucsc.edu]
#include<stdio.h>
#include<stdlib.h>
#include "List.h"
#include "insertionSort.h"

//performs insertionSort algorithm upon a given array, user can see changes in passed array
void insertionSort(int keys[], int numKeys){
	ListRef list = newList();
	for(int i = 0; i < numKeys; i++){
		sortedInsert(list, (long)keys[i]);
	}
	//printList(list);
	for(int i = 0; i < numKeys; i++){
		keys[i] = (int)getFirst(list);
		if(!isEmpty(list))
			deleteFirst(list);
	}
	freeList(list);
}

